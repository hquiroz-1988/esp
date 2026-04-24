import csv
from collections import Counter

file_path = '/home/blob/Documents/repos/esp/captures/quick_ads1115_check_20260422_234218/quick_ads1115_check_20260422_234218_i2c.csv'

type_counts = Counter()
keywords = ["address", "read", "write", "data", "ack"]
keyword_found = False

with open(file_path, mode='r') as f:
    reader = csv.reader(f)
    header = next(reader)
    
    for row in reader:
        if len(row) > 1:
            type_val = row[1].strip()
            type_counts[type_val] += 1
        
        row_str = ",".join(row).lower()
        if any(kw in row_str for kw in keywords):
            keyword_found = True

print(f"Unique values in column 2 (type) and counts: {dict(type_counts)}")
print(f"Are any keywords (address/read/write/data/ack) present in any rows? {'Yes' if keyword_found else 'No'}")
