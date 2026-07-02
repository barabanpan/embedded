import json
from datetime import datetime, timedelta


def lambda_handler(event, context):
    http_method = event.get("httpMethod")
    path = event.get("path")
    if http_method == "POST" and path == "/measurement":
        body = json.loads(event["body"])
        light_value = int(body.get("light"))
        voltage = float(body.get("volt"))
        dt = datetime.now()
        print(f"201 - {dt}, LIGHT_VALUE: {light_value}, VOLTAGE: {voltage}")
        return created()

    else:
        print(f"404 - http_method: {http_method}, path: {path}")
        return html_response("<h1>404</h1>")


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

