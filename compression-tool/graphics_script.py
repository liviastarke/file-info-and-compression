import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

df = pd.read_csv("test-files/compression-metrics.csv")
df["compressionRate"] = df["original"] / df["compressed"]
df["percentReduction"] = (df["original"] - df["compressed"]) / df["original"]
df["bitsPerChar"] = df["compressed"] / df["original"]


# Determine plot limits based on maximum file size
max_size = df["compressed"].max()
if pd.isna(max_size):
    max_size = 3000000

plt.figure(figsize=(8, 8))
plt.suptitle("Métricas da ferramenta de linha de comando OLIVE")

# Compression and decompression times in function of original file size
plt.subplot(2, 2, 1)
sns.regplot(df, x="original", y="timeCompression", label="Tempo de compressão")
sns.regplot(df, x="original", y="timeDecompression", label="Tempo de descompressão")
plt.title("Tempos de Compressão e Descompressão em função de Tamanho de Arquivo Original")
plt.xlabel("Tamanho do arquivo original")
plt.ylabel("Tempo de execução")
plt.legend()

# File sizes before and after compression
plt.subplot(2, 2, 2)
plt.title("Tamanhos de Arquivo antes e depois da Compressão")
sns.regplot(df, x="original", y="compressed", label="Arquivos de teste")
plt.xlabel("Tamanho do arquivo original")
plt.ylabel("Tamanho do arquivo comprimido")
plt.plot([0, max_size], [0, max_size], color='red', linestyle='dashed', label="Linha de referência representando nenhuma mudança no tamanho do arquivo")
plt.legend()

# Compression Rates in function of Original File Size
plt.subplot(2, 2, 3)
plt.title("Taxas de Compressão em função de Tamanho de Arquivo Original")
sns.regplot(df, x="original", y="compressionRate", label="Arquivos de teste")
plt.xlabel("Tamanho do arquivo original")
plt.ylabel("Taxa de compressão")

# Percent File Size Reduction in function of Original File Size
plt.subplot(2, 2, 4)
plt.title("Redução Percentual em função de Tamanho de Arquivo Original")
sns.regplot(df, x="original", y="percentReduction", label="Arquivos de teste")
plt.xlabel("Tamanho do arquivo original")
plt.ylabel("Redução percentual")


plt.show()
