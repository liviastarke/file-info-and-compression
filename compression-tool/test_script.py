import os
import subprocess
from pathlib import Path
import shlex
import time
import matplotlib.pyplot as plt

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
        lines.append("Compression execution status: " + "OK" if p.returncode == 0 else "ERROR")
        lines.append("Time to execute: " + str(execution_time) + " seconds")

        # File Size
        p = subprocess.run(f"du --bytes --apparent-size {shlex.quote(str(file))} | awk '{{print $1}}'", shell=True, capture_output=True, text=True)
        lines.append("File size: " + p.stdout.strip()) # strip removes blank chars like \n at the end of the output
        sizes["original"].append(int(p.stdout.strip()))

        # Compressed File Size
        p = subprocess.run(f"du --bytes --apparent-size test_out_file.olv | awk '{{print $1}}'", shell=True, capture_output=True, text=True)
        lines.append("Compressed size: " + p.stdout.strip())
        sizes["compressed"].append(int(p.stdout.strip()))

        # Decompression success and time
        start_time = time.perf_counter() # Record the start time
        p = subprocess.run(["./compression-tool", "-d", "test_out_file.olv"], capture_output=True)
        execution_time = time.perf_counter() - start_time
        lines.append("Decompression execution status: " + "OK" if p.returncode == 0 else "ERROR")
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


plt.scatter(sizes["original"], sizes["compressed"], label="Test files")
plt.title("File sizes before and after OLIVE compression")
plt.xlabel("Original file sizes")
plt.ylabel("Compressed file sizes")
plt.plot([0, 4000], [0, 4000], color='red', linestyle='dashed', label="Reference line representing no change in file size")
plt.legend()
plt.show()



