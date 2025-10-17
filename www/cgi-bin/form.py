#!/usr/bin/env python3

import cgi
import html
import sys

print("Content-type: text/html\r\n\r\n")

sys.stdin.reconfigure(encoding='utf-8')
sys.stdout.reconfigure(encoding='utf-8')

form = cgi.FieldStorage()
print("<!DOCTYPE html>")
print("<html lang='fr'>")
print("<head><meta charset='UTF-8'><title>Résultat POST Python</title></head>")
print("<body>")
print("<h1>Python POST</h1>")

if form:
    name = html.escape(form.getvalue("name", "Not provided"))
    data = html.escape(form.getvalue("message", "Not provided"))

    print("<h2>Received data:</h2>")
    print(f"<p><strong>Name:</strong> {name}</p>")
    print(f"<p><strong>Message:</strong> {data}</p>")
else:
    print("<p>No POST data received or form not submitted correctly.</p>")

print("</body>")
print("</html>")
