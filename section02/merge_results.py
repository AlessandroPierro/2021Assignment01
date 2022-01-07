import pandas as pd
import os

def adapt_column_names(df, name):
    name = name.replace('.csv', '')
    names = df.columns.values
    names[2] = "time_" + name
    names[3] = "bandwidth_" + name
    df.columns = names
    return df

# Read the list of CSV in the folder
csv_list = []
for file in os.listdir("./"):
    if file.endswith(".csv"):
        csv_list.append(file)

df = pd.read_csv(csv_list[0])
df["name"] = str(csv_list[0].replace('.csv', ''))

for i in range(1, len(csv_list)):
    df_new = pd.read_csv(csv_list[i])
    name = csv_list[i].replace('.csv', '')
    df_new["name"] = name
    df = df.append(df_new, ignore_index=True)

# Export to CSV
df.to_csv("complete_data.csv", index=False)