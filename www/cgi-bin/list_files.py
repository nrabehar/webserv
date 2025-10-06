#!/usr/bin/env python3#!/usr/bin/env python3

import cgi

import cgitbimport os

import osimport cgi

import jsonimport cgitb

import sys

import statcgitb.enable()

from datetime import datetime

print("Content-Type: text/html\n")

cgitb.enable()

upload_dir = '/home/nekena/lab/42/webserv/www/uploads/'

def send_response(status_code, content_type, content):

    print(f"Status: {status_code}")files = []

    print(f"Content-Type: {content_type}")if os.path.exists(upload_dir):

    print()    for f in os.listdir(upload_dir):

    print(content)        if os.path.isfile(os.path.join(upload_dir, f)):

            size = os.path.getsize(os.path.join(upload_dir, f))

def send_json_response(data):            files.append((f, size))

    send_response(200, "application/json", json.dumps(data))

print("""

def send_error_response(status_code, message):<div class="files-container">

    send_response(status_code, "application/json", json.dumps({"success": False, "error": message}))""")



def get_file_info(filepath):if files:

    """Get detailed information about a file"""    for name, size in files:

    try:        print(f"""

        stat_info = os.stat(filepath)    <div class="file-item">

        filename = os.path.basename(filepath)        <div class="file-info">

            <i class="fa-solid fa-file"></i>

        # Determine file type            <span>{name}</span>

        if os.path.isdir(filepath):            <span class="file-size">({size} bytes)</span>

            file_type = "directory"        </div>

        else:        <div class="file-actions">

            file_type = "file"            <a href="/uploads/{name}" target="_blank" class="action-btn view-btn"><i class="fa-solid fa-eye"></i> View</a>

            <a href="/cgi-bin/delete.py?file={name}" class="action-btn delete-btn"><i class="fa-solid fa-trash"></i> Delete</a>

        return {        </div>

            "name": filename,    </div>

            "type": file_type,        """)

            "size": stat_info.st_size,else:

            "modified": stat_info.st_mtime,    print("<p>No files uploaded yet.</p>")

            "permissions": oct(stat_info.st_mode)[-3:],

            "path": filepathprint("</div>")
        }
    except Exception as e:
        return None

def list_directory(directory_path):
    """List all files and directories in the given path"""
    try:
        if not os.path.exists(directory_path):
            return []

        items = []
        for item in os.listdir(directory_path):
            item_path = os.path.join(directory_path, item)
            file_info = get_file_info(item_path)
            if file_info:
                items.append(file_info)

        # Sort: directories first, then files, both alphabetically
        items.sort(key=lambda x: (x['type'] != 'directory', x['name'].lower()))
        return items

    except Exception as e:
        return []

def main():
    try:
        # Get query parameters
        query_string = os.environ.get('QUERY_STRING', '')
        params = {}
        if query_string:
            for param in query_string.split('&'):
                if '=' in param:
                    key, value = param.split('=', 1)
                    params[key] = value

        # Get the path to list
        requested_path = params.get('path', '/uploads/')

        # Ensure path is absolute and within web root
        if not requested_path.startswith('/'):
            requested_path = '/' + requested_path

        # Convert to filesystem path
        web_root = os.environ.get('DOCUMENT_ROOT', '/www')
        full_path = os.path.join(web_root, requested_path.lstrip('/'))

        # Security check: ensure path is within web root
        if not os.path.abspath(full_path).startswith(os.path.abspath(web_root)):
            send_error_response(403, "Access denied: Path outside web root")
            return

        # List the directory contents
        files = list_directory(full_path)

        response = {
            "success": True,
            "path": requested_path,
            "files": files,
            "count": len(files)
        }

        send_json_response(response)

    except Exception as e:
        send_error_response(500, f"Server error: {str(e)}")

if __name__ == "__main__":
    main()