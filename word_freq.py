import sqlite3

conn = sqlite3.connect('word_freq.db')
cursor = conn.cursor()

cursor.execute("SELECT * FROM word_freq;")
rows = cursor.fetchall()

for row in rows:
    print(f"Filename: {row[1]}, Word: {row[2]}, Frequency: {row[3]}")

conn.close()
