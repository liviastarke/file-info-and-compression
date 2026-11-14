import matplotlib.pyplot as plt
import pandas as pd

# O CSV deve estar na mesma pasta deste script.
df = pd.read_csv("resultados_compressao.csv")

# Mantém somente os seis arquivos da amostra principal.
df = df[df["arquivo"].isin([
    "01_texto_pequeno.txt",
    "02_texto_repetitivo.txt",
    "03_texto_variado.txt",
    "04_poucas_repeticoes.txt",
    "05_bytes_256.bin",
    "06_dados_aleatorios.bin",
])].copy()

labels = [
    "Texto\npequeno", "Texto\nrepetitivo", "Texto\nvariado",
    "Poucas\nrepetições", "256 bytes\ndistintos", "Dados\naleatórios"
]
x = range(len(df))

# Gráfico 1: tamanhos em cada etapa.
fig, ax = plt.subplots(figsize=(11, 6))
width = 0.25
ax.bar([i - width for i in x], df["original_bytes"], width, label="Original")
ax.bar(list(x), df["apos_lz77_bytes"], width, label="Após LZ77")
ax.bar([i + width for i in x], df["olive_final_bytes"], width, label="OLIVE final")
ax.set_yscale("log")
ax.set_ylabel("Tamanho (bytes, escala logarítmica)")
ax.set_title("Tamanho dos arquivos em cada etapa da compressão")
ax.set_xticks(list(x), labels)
ax.legend()
ax.grid(axis="y", alpha=0.25)
fig.tight_layout()
fig.savefig("grafico_tamanhos_etapas.png", dpi=200)
plt.show()

# Gráfico 2: redução percentual.
fig, ax = plt.subplots(figsize=(11, 6))
width = 0.35
ax.bar([i - width/2 for i in x], df["reducao_lz77_percentual"], width, label="Após LZ77")
ax.bar([i + width/2 for i in x], df["reducao_final_percentual"], width, label="OLIVE final")
ax.axhline(0, linewidth=1)
ax.set_ylabel("Variação em relação ao original (%)")
ax.set_title("Redução ou aumento do tamanho dos arquivos")
ax.set_xticks(list(x), labels)
ax.legend()
ax.grid(axis="y", alpha=0.25)
fig.tight_layout()
fig.savefig("grafico_reducao_percentual.png", dpi=200)
plt.show()

# Gráfico 3: tempos em milissegundos.
fig, ax = plt.subplots(figsize=(11, 6))
ax.bar([i - width/2 for i in x], df["tempo_compressao"] * 1000, width, label="Compressão")
ax.bar([i + width/2 for i in x], df["tempo_descompressao"] * 1000, width, label="Descompressão")
ax.set_ylabel("Tempo (milissegundos)")
ax.set_title("Tempo de compressão e descompressão")
ax.set_xticks(list(x), labels)
ax.legend()
ax.grid(axis="y", alpha=0.25)
fig.tight_layout()
fig.savefig("grafico_tempos.png", dpi=200)
plt.show()
