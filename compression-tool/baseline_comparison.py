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
data = {
    "filename": [],
    "original_size": [],
    "gzip_compressed": [],
    "gzip_comp_time": [],
    "gzip_decomp_time": [],
    "xz_compressed": [],
    "xz_comp_time": [],
    "xz_decomp_time": []
}

# Unique output files for this script
gz_out = "test_out_file.gz"
gz_recon = "test_reconstruct_gzip"
xz_out = "test_out_file.xz"
xz_recon = "test_reconstruct_xz"

for file in folder.iterdir():
    if file.is_file():
        # Avoid processing output files if they get saved in the same directory
        if file.name in [gz_out, gz_recon, xz_out, xz_recon, "compression-metrics.csv", "gzip_xz_metrics.csv"]:
            continue
            
        lines.append(f"--- {file.name} ---")
        data["filename"].append(file.name)

        # Original File Size
        orig_size = os.path.getsize(file)
        lines.append(f"Original size: {orig_size}")
        data["original_size"].append(orig_size)

        # ==========================================
        # GZIP TESTING
        # ==========================================
        # Compress
        start_time = time.perf_counter()
        with open(gz_out, "wb") as f_out:
            p_comp = subprocess.run(["gzip", "-c", str(file)], stdout=f_out)
        gz_comp_time = time.perf_counter() - start_time
        
        gz_size = os.path.getsize(gz_out)
        data["gzip_compressed"].append(gz_size)
        data["gzip_comp_time"].append(gz_comp_time)
        
        lines.append("GZIP Compression: " + ("OK" if p_comp.returncode == 0 else "ERROR"))
        lines.append(f"GZIP Compressed size: {gz_size}")
        lines.append(f"GZIP Comp Time: {gz_comp_time:.6f} s")

        # Decompress
        start_time = time.perf_counter()
        with open(gz_recon, "wb") as f_recon:
            p_decomp = subprocess.run(["gzip", "-d", "-c", gz_out], stdout=f_recon)
        gz_decomp_time = time.perf_counter() - start_time
        data["gzip_decomp_time"].append(gz_decomp_time)
        
        # Verify
        p_diff = subprocess.run(f"diff -q {shlex.quote(str(file))} {gz_recon}", shell=True, capture_output=True, text=True)
        lines.append("GZIP Integrity: " + ("OK" if p_diff.returncode == 0 else f"ERROR - {p_diff.stdout.strip()}"))

        # ==========================================
        # XZ TESTING
        # ==========================================
        # Compress
        start_time = time.perf_counter()
        with open(xz_out, "wb") as f_out:
            p_comp = subprocess.run(["xz", "-c", str(file)], stdout=f_out)
        xz_comp_time = time.perf_counter() - start_time
        
        xz_size = os.path.getsize(xz_out)
        data["xz_compressed"].append(xz_size)
        data["xz_comp_time"].append(xz_comp_time)
        
        lines.append("XZ Compression: " + ("OK" if p_comp.returncode == 0 else "ERROR"))
        lines.append(f"XZ Compressed size: {xz_size}")
        lines.append(f"XZ Comp Time: {xz_comp_time:.6f} s")

        # Decompress
        start_time = time.perf_counter()
        with open(xz_recon, "wb") as f_recon:
            p_decomp = subprocess.run(["xz", "-d", "-c", xz_out], stdout=f_recon)
        xz_decomp_time = time.perf_counter() - start_time
        data["xz_decomp_time"].append(xz_decomp_time)
        
        # Verify
        p_diff = subprocess.run(f"diff -q {shlex.quote(str(file))} {xz_recon}", shell=True, capture_output=True, text=True)
        lines.append("XZ Integrity: " + ("OK" if p_diff.returncode == 0 else f"ERROR - {p_diff.stdout.strip()}"))
        lines.append("") # Spacing

# ==========================================
# CLEANUP TEMPORARY FILES
# ==========================================
# List of the temporary files we created during the tests
temp_files_to_delete = [gz_out, gz_recon, xz_out, xz_recon]

for temp_file in temp_files_to_delete:
    try:
        os.remove(temp_file)
        # Or if you are using Path objects: Path(temp_file).unlink()
    except FileNotFoundError:
        # If the file doesn't exist (e.g., if a compression failed), just ignore and continue
        pass

print("Cleanup complete. Generating plots...")

with open("test_report_gzip_xz.txt", mode='w') as output:
    output.write("\n".join(lines))

# ==========================================
# DATA PROCESSING & PLOTTING
# ==========================================
df = pd.DataFrame(data)

# Calculate metrics for GZIP
df["gzip_compressionRate"] = df["original_size"] / df["gzip_compressed"]
df["gzip_percentReduction"] = (df["original_size"] - df["gzip_compressed"]) / df["original_size"]

# Calculate metrics for XZ
df["xz_compressionRate"] = df["original_size"] / df["xz_compressed"]
df["xz_percentReduction"] = (df["original_size"] - df["xz_compressed"]) / df["original_size"]

# Determine plot limits based on maximum file size
max_size = df["original_size"].max()
if pd.isna(max_size):
    max_size = 3000000

plt.figure(figsize=(12, 10))

# 1. Base Matplotlib Scatter
plt.subplot(2, 2, 1)
plt.scatter(df["original_size"], df["gzip_compressed"], label="GZIP", alpha=0.7)
plt.scatter(df["original_size"], df["xz_compressed"], label="XZ", alpha=0.7)
plt.title("File sizes before and after Compression")
plt.xlabel("Original file sizes")
plt.ylabel("Compressed file sizes")
plt.plot([0, max_size], [0, max_size], color='red', linestyle='dashed', label="No change")
plt.legend()

# 2. Seaborn Regplot (Sizes)
plt.subplot(2, 2, 2)
plt.title("File sizes before and after Compression (Seaborn)")
sns.regplot(data=df, x="original_size", y="gzip_compressed", label="GZIP")
sns.regplot(data=df, x="original_size", y="xz_compressed", label="XZ")
plt.xlabel("Original file sizes")
plt.ylabel("Compressed file sizes")
plt.plot([0, max_size], [0, max_size], color='red', linestyle='dashed', label="No change")
plt.legend()

# 3. Compression Rates
plt.subplot(2, 2, 3)
plt.title("Compression Rates by Original File Size")
sns.regplot(data=df, x="original_size", y="gzip_compressionRate", label="GZIP")
sns.regplot(data=df, x="original_size", y="xz_compressionRate", label="XZ")
plt.xlabel("Original file sizes")
plt.ylabel("Compression rate")
plt.legend()

# 4. Percent Reduction
plt.subplot(2, 2, 4)
plt.title("Percent Size Reduction by Original File Size")
sns.regplot(data=df, x="original_size", y="gzip_percentReduction", label="GZIP")
sns.regplot(data=df, x="original_size", y="xz_percentReduction", label="XZ")
plt.xlabel("Original file sizes")
plt.ylabel("Percent Reduction")
plt.legend()

plt.tight_layout()
plt.show()

# Save final metrics
df.to_csv("test-files/gzip_xz_metrics.csv", index=False)
