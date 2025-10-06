#!/usr/bin/env python3#!/usr/bin/env python3

import cgi

import cgitbimport cgi

import osimport cgitb

import jsonimport os

import sys

from datetime import datetimecgitb.enable()



cgitb.enable()print("Content-Type: text/html\n")



def send_response(status_code, content_type, content):form = cgi.FieldStorage()

    print(f"Status: {status_code}")

    print(f"Content-Type: {content_type}")if 'file' in form:

    print()    fileitem = form['file']

    print(content)    if fileitem.filename:

        filename = os.path.basename(fileitem.filename)

def send_json_response(data):        with open('/home/nekena/lab/42/webserv/www/uploads/' + filename, 'wb') as f:

    send_response(200, "application/json", json.dumps(data))            f.write(fileitem.file.read())

        message = "File '{}' uploaded successfully!".format(filename)

def send_error_response(status_code, message):    else:

    send_response(status_code, "application/json", json.dumps({"success": False, "error": message}))        message = "No file selected."

else:

def ensure_directory_exists(directory):    message = "No file uploaded."

    """Create directory if it doesn't exist"""

    if not os.path.exists(directory):print("""

        os.makedirs(directory, exist_ok=True)<!DOCTYPE html>

<html lang="en">

def save_uploaded_file(file_item, upload_path):<head>

    """Save an uploaded file to the specified path"""    <meta charset="UTF-8">

    if not file_item.filename:    <title>Upload Result</title>

        return None, "No filename provided"    <style>

        body { font-family: Arial, sans-serif; background: #181A20; color: #E5E7EB; text-align: center; padding: 50px; }

    filename = os.path.basename(file_item.filename)        h1 { color: #109E82; }

    filepath = os.path.join(upload_path, filename)        p { margin: 20px 0; }

        a { color: #4348D5; text-decoration: none; }

    # Prevent directory traversal    </style>

    if ".." in filename or "/" in filename or "\\" in filename:</head>

        return None, "Invalid filename"<body>

    <h1>Upload Result</h1>

    try:    <p>{}</p>

        with open(filepath, 'wb') as f:    <p><a href="/forms.html">Upload Another File</a> | <a href="/">Back to Home</a></p>

            while True:</body>

                chunk = file_item.file.read(8192)</html>

                if not chunk:""".format(message))
                    break
                f.write(chunk)
        return filepath, None
    except Exception as e:
        return None, f"Failed to save file: {str(e)}"

def main():
    try:
        # Get form data
        form = cgi.FieldStorage()

        # Get upload path
        upload_path = form.getvalue('path', '/uploads/')

        # Ensure upload path is absolute and within web root
        if not upload_path.startswith('/'):
            upload_path = '/' + upload_path

        # Convert to filesystem path
        web_root = os.environ.get('DOCUMENT_ROOT', '/www')
        full_upload_path = os.path.join(web_root, upload_path.lstrip('/'))

        # Ensure the upload directory exists
        ensure_directory_exists(full_upload_path)

        uploaded_files = []
        errors = []

        # Process uploaded files
        if 'files' in form:
            files = form['files']
            if not isinstance(files, list):
                files = [files]

            for file_item in files:
                if file_item.filename:
                    filepath, error = save_uploaded_file(file_item, full_upload_path)
                    if filepath:
                        uploaded_files.append({
                            "filename": os.path.basename(filepath),
                            "path": filepath,
                            "size": os.path.getsize(filepath)
                        })
                    else:
                        errors.append(f"Failed to upload {file_item.filename}: {error}")

        if not uploaded_files and not errors:
            send_error_response(400, "No files were uploaded")
            return

        # Prepare response
        response = {
            "success": len(errors) == 0,
            "uploaded": uploaded_files,
            "errors": errors,
            "message": f"Successfully uploaded {len(uploaded_files)} file(s)" if uploaded_files else "Upload completed with errors"
        }

        send_json_response(response)

    except Exception as e:
        send_error_response(500, f"Server error: {str(e)}")

if __name__ == "__main__":
    main()