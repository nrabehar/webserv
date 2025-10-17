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
print("<h1>Réception de la méthode POST (Python CGI)</h1>")

if form:
    username = html.escape(form.getvalue("username_post", "Non fourni"))
    data = html.escape(form.getvalue("data_post", "Non fourni"))

    print("<h2>Données reçues:</h2>")
    print(f"<p><strong>Nom (username_post):</strong> {username}</p>")
    print(f"<p><strong>Message (data_post):</strong> {data}</p>")
else:
    print("<p>Aucune donnée POST reçue ou formulaire mal soumis.</p>")

print("</body>")
print("</html>")
