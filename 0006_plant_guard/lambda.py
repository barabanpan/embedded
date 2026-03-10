import io
import os
import csv
import uuid
import json
import boto3
from datetime import datetime, timedelta
from collections import defaultdict

s3 = boto3.client("s3")

DEVICE_API_KEY = os.environ.get("DEVICE_API_KEY")
BUCKET_NAME = os.environ.get("BUCKET_NAME")
KEY = "sessions.json"

EXAMPLE_SESSIONS = [
    # 5 on 28th
    {
        "id": "1add",
        "start": 1772241849,
        "duration": 8,
        "min_distance": 19,
    },
    {
        "id": "6a4b",
        "start": 1772242509,
        "duration": 12,
        "min_distance": 17,
    },
    {
        "id": "04d8",
        "start": 1772243471,
        "duration": 16,
        "min_distance": 7,
    },
    {
        "id": "05a2",
        "start": 1772244835,
        "duration": 21,
        "min_distance": 12,
    },
    {
        "id": "f40b",
        "start": 1772298895,
        "duration": 8,
        "min_distance": 16,
    },
    # 3 on 1st
    {
        "id": "a8f3",
        "start": 1772330873,
        "duration": 22,
        "min_distance": 12,
    },
    {
        "id": "7ee8",
        "start": 1772330973,
        "duration": 20,
        "min_distance": 14,
    },
    {
        "id": "14ab",
        "start": 1772381701,
        "duration": 45,
        "min_distance": 7,
    },
    # 4 on 2nd
    {
        "id": "901d",
        "start": 1772414533,
        "duration": 11,
        "min_distance": 19,
    },
    {
        "id": "23a3",
        "start": 1772420361,
        "duration": 39,
        "min_distance": 14,
    },
    {
        "id": "b4ea",
        "start": 1772476881,
        "duration": 13,
        "min_distance": 17,
    },
    {
        "id": "10f4",
        "start": 1772476991,
        "duration": 29,
        "min_distance": 7,
    },
    # 0 on 3rd
    # 2 on 4th
    {
        "id": "0c3f",
        "start": 1772594109,
        "duration": 9,
        "min_distance": 15,
    },
    {
        "id": "852a",
        "start": 1772594502,
        "duration": 13,
        "min_distance": 14,
    },
    # 1 on 5th
    {
        "id": "1cba",
        "start": 1772684649,
        "duration": 9,
        "min_distance": 17,
    },
]

def lambda_handler(event, context):
    http_method = event.get("httpMethod")
    path = event.get("path")
    if http_method == "POST" and path == "/event":
        api_key = (event.get("headers") or {}).get("DEVICE_API_KEY")
        if api_key != DEVICE_API_KEY:
            return unauthorized()
        
        body = json.loads(event["body"])
        min_distance = int(body.get("min_dist"))
        duration = int(body.get("duration"))
        start = int(datetime.now().timestamp()) - duration
        write_session(start, duration, min_distance)
        return created()

    elif http_method == "GET" and path == "/dashboard":
        demo = (event.get("queryStringParameters") or {}).get("demo") == "true"
        if demo:
            return dashboard(EXAMPLE_SESSIONS)
        
        sessions = read_sessions()
        return dashboard(sessions)
    
    else:
        return html_response("<h1>404</h1>")

def write_session(start, duration, min_distance):
    new_session = {
        "id": str(uuid.uuid4()),
        "start": start,
        "duration": duration,
        "min_distance": min_distance,
    }

    sessions = read_sessions()
    sessions.append(new_session)
    
    output = io.StringIO()
    fieldnames = ["id", "start", "duration", "min_distance"]

    writer = csv.DictWriter(output, fieldnames=fieldnames)
    writer.writeheader()
    writer.writerows(sessions)

    s3.put_object(
        Bucket=BUCKET_NAME,
        Key=KEY,
        Body=output.getvalue(),
        ContentType="text/csv"
    )

def read_sessions():
    try:
        obj = s3.get_object(Bucket=BUCKET_NAME, Key=KEY)
        csv_content = obj["Body"].read().decode("utf-8")
        sessions = list(csv.DictReader(io.StringIO(csv_content)))
    except s3.exceptions.NoSuchKey:
        sessions = []
    for s in sessions:
        s["start"] = int(s["start"])
        s["duration"] = int(s["duration"])
        s["min_distance"] = int(s["min_distance"])
    return sessions
  
def dashboard(sessions):
    if not sessions:
        return html_response(empty_dashboard())

    # ---- Basic stats ----
    total_sessions = len(sessions)
    longest_session = max(s["duration"] for s in sessions)
    average_duration = round(
        sum(s["duration"] for s in sessions) / total_sessions, 2
    )

    # ---- Prepare data ----
    for s in sessions:
        s["date"] = datetime.utcfromtimestamp(s["start"]).date()
        s["hour"] = datetime.utcfromtimestamp(s["start"]).hour

    # ---- Date range including empty days ----
    min_date = min(s["date"] for s in sessions)
    max_date = max(s["date"] for s in sessions)

    cur_date = min_date
    all_dates = []
    while cur_date <= max_date:
        all_dates.append(cur_date)
        cur_date += timedelta(days=1)
    
    # ---- Sessions per day ----
    sessions_per_day = defaultdict(int)
    total_duration_per_day = defaultdict(int)
    for s in sessions:
        sessions_per_day[s["date"]] += 1
        total_duration_per_day[s["date"]] += s["duration"]

    daily_sessions = [sessions_per_day[d] for d in all_dates]
    daily_total_duration = [total_duration_per_day[d] for d in all_dates]

     # ---- Duration of each session ----
    durations = [s["duration"] for s in sessions]

    # ---- Min distance of each session ----
    min_distances = [s["min_distance"] for s in sessions]

    # ---- Heatmap (date x hour) ----
    heatmap = defaultdict(lambda: [0]*24)
    for s in sessions:
        heatmap[s["date"]][s["hour"]] += 1

    heatmap_matrix = [heatmap[d] for d in all_dates]

    html = f"""<!DOCTYPE html>
<html>
<head>
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<script src="https://cdn.jsdelivr.net/npm/chartjs-chart-matrix@3.0.0"></script>
<style>
body {{
    font-family: Arial;
    margin: 30px;
}}
.grid {{
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 30px;
}}
.card {{
    padding: 15px;
    border: 1px solid #ddd;
    border-radius: 8px;
}}
.stat-box {{
    display: flex;
    gap: 30px;
    margin-bottom: 30px;
}}
.stat {{
    padding: 10px 20px;
    border: 1px solid #ccc;
    border-radius: 6px;
}}
canvas {{
    height: 250px !important;
}}
</style>
</head>
<body>

<h1>Cat Activity Dashboard</h1>

<div class="stat-box">
    <div class="stat">Total Sessions: {total_sessions}</div>
    <div class="stat">Longest: {longest_session}s</div>
    <div class="stat">Average: {average_duration}s</div>
</div>

<div class="grid">
    <div class="card">
        <h3>Sessions per Day</h3>
        <canvas id="dailyChart"></canvas>
    </div>

    <div class="card">
        <h3>Duration per Day</h3>
        <canvas id="dailyDurationChart"></canvas>
    </div>

    <div class="card">
        <h3>Session Durations</h3>
        <canvas id="sessionDurationChart"></canvas>
    </div>

    <div class="card">
        <h3>Closest Distance per Session</h3>
        <canvas id="distanceChart"></canvas>
    </div>

    <div class="card">
        <h3>Sessions per Hour Heatmap</h3>
        <canvas id="heatmapChart"></canvas>
    </div>
</div>

<script>
const dates = {json.dumps([str(d) for d in all_dates])};
const dailyValues = {json.dumps(daily_sessions)};
const totalDailyDurations = {json.dumps(daily_total_duration)};
const durations = {json.dumps(durations)};
const minDistances = {json.dumps(min_distances)};
const heatmapData = {json.dumps(heatmap_matrix)};

// ---- Sessions per day (LINE)
new Chart(document.getElementById('dailyChart'), {{
    type: 'line',
    data: {{
        labels: dates,
        datasets: [{{
            label: 'Sessions',
            data: dailyValues,
            borderWidth: 2,
            fill: false
        }}]
    }}
}});

// ---- Total duration per day (LINE)
new Chart(document.getElementById('dailyDurationChart'), {{
    type: 'line',
    data: {{
        labels: dates,
        datasets: [{{
            label: 'Sessions',
            data: totalDailyDurations,
            borderWidth: 2,
            fill: false
        }}]
    }}
}});

// ---- Duration per session (BAR)
new Chart(document.getElementById('sessionDurationChart'), {{
    type: 'bar',
    data: {{
        labels: durations.map((_, i) => 'S' + (i+1)),
        datasets: [{{
            label: 'Duration (sec)',
            data: durations
        }}]
    }}
}});

// ---- Min distance (LINE)
new Chart(document.getElementById('distanceChart'), {{
    type: 'line',
    data: {{
        labels: minDistances.map((_, i) => 'S' + (i+1)),
        datasets: [{{
            label: 'Min Distance (cm)',
            data: minDistances
        }}]
    }}
}});

// ---- Sessions per hour (MATRIX)
const matrixData = [];

dates.forEach((date, dateIndex) => {{
    for (let hour = 0; hour < 24; hour++) {{
        matrixData.push({{
            x: date,
            y: hour,
            v: heatmapData[dateIndex][hour]
        }});
    }}
}});

new Chart(document.getElementById('heatmapChart'), {{
    type: 'matrix',
    data: {{
        datasets: [{{
            data: matrixData,
            backgroundColor: ctx => {{
                const value = ctx.raw.v;
                const alpha = Math.min(value * 0.4, 1);
                return `rgba(0, 0, 255, ${{alpha}})`;
            }}
        }}]
    }},
    options: {{
        scales: {{
            x: {{
                type: 'category',
                labels: dates,
                offset: true
            }},
            y: {{
                type: 'linear',
                min: 0,
                max: 23,
                ticks: {{ stepSize: 4 }}
            }}
        }},
        plugins: {{
            legend: {{ display: false }}
        }}
    }}
}});
</script>

</body>
</html>
"""

    return html_response(html)

def unauthorized():
    return {
        "statusCode": 401,
        "body": json.dumps({
            "message": "Invalid API key"
        }),
    }

def created():
    return {
        "statusCode": 201,
    }

def html_response(body):
    return {
        "statusCode": 200,
        "headers": {"Content-Type": "text/html"},
        "body": body,
    }


def empty_dashboard():
    return """
    <html>
      <body>
        <h2>No plant activity detected yet.</h2>
      </body>
    </html>
    """


