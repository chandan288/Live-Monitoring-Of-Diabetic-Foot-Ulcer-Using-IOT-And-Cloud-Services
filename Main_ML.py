import pandas as pd
import requests
from pushbullet import Pushbullet

# ThingSpeak API URL and key
THINGSPEAK_API_URL = "https://api.thingspeak.com/channels/2125222/feeds.json?api_key=GZ7K0S4NMRREE2QI"

# Pushbullet API key 
PUSHBULLET_API_KEY = "o.hzWGKtO0JQx8JwymwTEASyQVvUf81tTC"

def get_data_from_thingspeak():
    response = requests.get(THINGSPEAK_API_URL)
    data = response.json()
    df = pd.DataFrame(data['feeds'])
    df = df.tail(1)  #
    return df

def predict_pressure_points(df):
    fields = ['field1', 'field2', 'field3', 'field4', 'field5', 'field6']
    for field in fields:
        df[field] = pd.to_numeric(df[field], errors='coerce')  # Convert field to int
        df[f'{field}_status'] = df[field].apply(lambda x: 'Abnormal' if x < 100 or x > 200 else 'Normal')  # Apply threshold
    return df

def send_notification(df):
    pb = Pushbullet(PUSHBULLET_API_KEY)
    
    for index, row in df.iterrows():
        if 'Abnormal' in row.values and row['field1'] < 200:
            message = "Abnormal Pressure Detected. Do consult your doctor."
            push = pb.push_note("Abnormal Pressure Alert", message)
            break  # Only print message once for each row
    
    else:
        push = pb.push_note("Pressure Points", "Pressure points are Normal. Don't worry, have a cookie!")

def update_excel_with_thingspeak(df):
    df.to_excel("your_excel_file.xlsx")

def main():
    df = get_data_from_thingspeak()
    df = predict_pressure_points(df)
    update_excel_with_thingspeak(df)
    
    # Read the updated values from the Excel file
    df_updated = pd.read_excel("your_excel_file.xlsx")
    
    # Predict pressure points with the updated DataFrame
    df_updated = predict_pressure_points(df_updated)
    
    # Continue processing with the updated DataFrame
    send_notification(df_updated)

if __name__ == "__main__":
    main()
