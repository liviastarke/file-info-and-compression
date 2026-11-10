import os
import subprocess
from pathlib import Path
import shlex
import time

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

folder = Path("./test-files") # substitute by your directory path

lines = []
sizes = {"original":[], "compressed":[]}
for file in folder.iterdir():
    if file.is_file():
        # File Name
        lines.append(file.name)

        # Compression command return code and time
        start_time = time.perf_counter() # Record the start time
        p = subprocess.run(["./compression-tool", "-c", file], capture_output=True)
        execution_time = time.perf_counter() - start_time
        lines.append("Compression execution status: " + ("OK" if p.returncode == 0 else "ERROR"))
        lines.append("Time to execute: " + str(execution_time) + " seconds")

        # File Size
        size = os.path.getsize(file)
        lines.append("File size: " + str(size))
        sizes["original"].append(size)

        # Compressed File Size
        size = os.path.getsize("test_out_file.olv")
        lines.append("Compressed size: " + str(size))
        sizes["compressed"].append(size)

        # Decompression success and time
        start_time = time.perf_counter() # Record the start time
        p = subprocess.run(["./compression-tool", "-d", "test_out_file.olv"], capture_output=True)
        execution_time = time.perf_counter() - start_time
        lines.append("Decompression execution status: " + ("OK" if p.returncode == 0 else "ERROR"))
        lines.append("Time to execute: " + str(execution_time) + " seconds")
        # Report if decompression did not produce identical file
        p = subprocess.run(f"diff -q {shlex.quote(str(file))} test_reconstruct_in_file", shell=True, capture_output=True, text=True)
        lines.append(p.stdout)

with open("test_report.txt", mode='w') as output:
    output.write("\n".join(lines))

# time ./compression-tool -c "$file"
# du --bytes --apparent-size "$file" | awk '{print $1}'
# du --bytes --apparent-size "test_out_file.olv" | awk '{print $1}'
# time ./compression-tool -d "test_out_file.olv"
# diff -s "$file" "test_reconstruct_in_file"


df = pd.DataFrame.from_dict(sizes)
df["compressionRate"] = df["original"] / df["compressed"]
df["percentReduction"] = (df["original"] - df["compressed"]) / df["original"]
df["bitsPerChar"] = df["compressed"] / df["original"]

plt.figure(figsize=(8, 8))

plt.subplot(2, 2, 1)
plt.scatter(sizes["original"], sizes["compressed"], label="Test files")
plt.title("File sizes before and after OLIVE compression")
plt.xlabel("Original file sizes")
plt.ylabel("Compressed file sizes")
plt.plot([0, 3000000], [0, 3000000], color='red', linestyle='dashed', label="Reference line representing no change in file size")
plt.legend()

plt.subplot(2, 2, 2)
plt.title("File sizes before and after OLIVE compression (Seaborn plot)")
sns.regplot(df, x="original", y="compressed", label="Test files")
plt.xlabel("Original file sizes")
plt.ylabel("Compressed file sizes")
plt.plot([0, 3000000], [0, 3000000], color='red', linestyle='dashed', label="Reference line representing no change in file size")
plt.legend()

plt.subplot(2, 2, 3)
plt.title("OLIVE Compression Rates in function of Original File Size (Seaborn plot)")
sns.regplot(df, x="original", y="compressionRate", label="Test files")
plt.xlabel("Original file sizes")
plt.ylabel("Compression rate")

plt.subplot(2, 2, 4)
plt.title("OLIVE Percent File Size Reduction in function of Original File Size (Seaborn plot)")
sns.regplot(df, x="original", y="percentReduction", label="Test files")
plt.xlabel("Original file sizes")
plt.ylabel("Percent Reduction")


plt.show()

df.to_csv("test-files/compression-metrics.csv")

