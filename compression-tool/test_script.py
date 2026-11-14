
import csv
import filecmp
import subprocess
import time
import sys
from pathlib import Path

# Diretório do projeto
BASE_DIR = Path(__file__).resolve().parent
TEST_DIR = BASE_DIR / "test-files"

# Executável e arquivos temporários
PROGRAM = BASE_DIR / "compression-tool"
COMPRESSED = BASE_DIR / "test_out_file.olv"
RECONSTRUCTED = BASE_DIR / "test_reconstruct_in_file"

results = []

# Arquivos da pasta de testes
files = sorted(TEST_DIR.iterdir())

if not files:
    raise SystemExit("Nenhum arquivo encontrado em test-files.")

for file in files:
    if not file.is_file():
        continue

    print(f"\nTestando: {file.name}")

    # Remove resultados anteriores
    COMPRESSED.unlink(missing_ok=True)
    RECONSTRUCTED.unlink(missing_ok=True)

    original_size = file.stat().st_size

    # Compressão
    start = time.perf_counter()
    compression = subprocess.run(
        [str(PROGRAM), "-c", str(file)],
        capture_output=True,
        text=True
    )
    compression_time = time.perf_counter() - start

    if compression.returncode != 0 or not COMPRESSED.exists():
        print("Erro na compressão.")
        continue

    compressed_size = COMPRESSED.stat().st_size

    # Recupera o tamanho dos dados após o LZ77,
    # armazenado no cabeçalho OLIVE.
    header = COMPRESSED.read_bytes()

    if len(header) < 15 or header[:5] != b"OLIVE":
        print("Erro: cabeçalho OLIVE inválido.")
        continue

    lz77_size = int.from_bytes(
        header[5:13],
        byteorder=sys.byteorder
    )

    # Descompressão
    start = time.perf_counter()
    decompression = subprocess.run(
        [str(PROGRAM), "-d", str(COMPRESSED)],
        capture_output=True,
        text=True
    )
    decompression_time = time.perf_counter() - start

    # Verificação da integridade
    integrity = (
        decompression.returncode == 0
        and RECONSTRUCTED.exists()
        and filecmp.cmp(file, RECONSTRUCTED, shallow=False)
    )

    # Cálculo das reduções percentuais
    lz77_reduction = (
        (original_size - lz77_size) / original_size * 100
        if original_size else None
    )

    final_reduction = (
        (original_size - compressed_size) / original_size * 100
        if original_size else None
    )

    result = {
        "arquivo": file.name,
        "original_bytes": original_size,
        "apos_lz77_bytes": lz77_size,
        "olive_final_bytes": compressed_size,
        "reducao_lz77_percentual": (
            round(lz77_reduction, 2)
            if lz77_reduction is not None else ""
        ),
        "reducao_final_percentual": (
            round(final_reduction, 2)
            if final_reduction is not None else ""
        ),
        "tempo_compressao": round(compression_time, 6),
        "tempo_descompressao": round(decompression_time, 6),
        "integridade": "OK" if integrity else "FALHA"
    }

    results.append(result)

    print(f"Original: {original_size} bytes")
    print(f"Após LZ77: {lz77_size} bytes")
    print(f"OLIVE final: {compressed_size} bytes")
    print(f"Integridade: {'OK' if integrity else 'FALHA'}")

# Salva os resultados em CSV
if results:
    output = BASE_DIR / "resultados_compressao.csv"

    with open(output, "w", newline="", encoding="utf-8") as csvfile:
        writer = csv.DictWriter(
            csvfile,
            fieldnames=results[0].keys()
        )
        writer.writeheader()
        writer.writerows(results)

    print(f"\nResultados salvos em: {output}")
else:
    print("\nNenhum resultado foi registrado.")
