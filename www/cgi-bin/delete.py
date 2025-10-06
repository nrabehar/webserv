#!/usr/bin/env python3#!/usr/bin/env python3

import cgi

import cgitbimport cgi

import osimport cgitb

import jsonimport os

import sys

import shutilcgitb.enable()



cgitb.enable()print("Content-Type: text/html\n")



def send_response(status_code, content_type, content):form = cgi.FieldStorage()

    print(f"Status: {status_code}")filename = form.getvalue('file', '')

    print(f"Content-Type: {content_type}")

    print()upload_dir = '/home/nekena/lab/42/webserv/www/uploads/'

    print(content)filepath = os.path.join(upload_dir, filename)



def send_json_response(data):if filename and os.path.exists(filepath):

    send_response(200, "application/json", json.dumps(data))    os.remove(filepath)

    print("Location: /file_manager.html?deleted=1\n\n")

def send_error_response(status_code, message):else:

    send_response(status_code, "application/json", json.dumps({"success": False, "error": message}))    print("Location: /file_manager.html?error=1\n\n")

def delete_file_or_directory(filepath, is_directory=False):
    """Delete a file or directory"""
    try:
        if not os.path.exists(filepath):
            return False, "File or directory does not exist"

        if is_directory:
            if not os.path.isdir(filepath):
                return False, "Path is not a directory"
            shutil.rmtree(filepath)
        else:
            if not os.path.isfile(filepath):
                return False, "Path is not a file"
            os.remove(filepath)

        return True, None
    except Exception as e:
        return False, f"Failed to delete: {str(e)}"

def main():
    try:
        # Only allow DELETE method
        if os.environ.get('REQUEST_METHOD') != 'DELETE':
            send_error_response(405, "Method not allowed")
            return

        # Read JSON data from stdin (for DELETE requests)
        content_length = int(os.environ.get('CONTENT_LENGTH', 0))
        if content_length > 0:
            post_data = sys.stdin.read(content_length)
            try:
                data = json.loads(post_data)
            except json.JSONDecodeError:
                send_error_response(400, "Invalid JSON data")
                return
        else:
            send_error_response(400, "No data provided")
            return

        # Get parameters
        file_path = data.get('path')
        is_directory = data.get('is_directory', False)

        if not file_path:
            send_error_response(400, "No file path provided")
            return

        # Ensure path is absolute and within web root
        if not file_path.startswith('/'):
            file_path = '/' + file_path

        # Convert to filesystem path
        web_root = os.environ.get('DOCUMENT_ROOT', '/www')
        full_path = os.path.join(web_root, file_path.lstrip('/'))

        # Security check: ensure path is within web root
        if not os.path.abspath(full_path).startswith(os.path.abspath(web_root)):
            send_error_response(403, "Access denied: Path outside web root")
            return

        # Delete the file or directory
        success, error = delete_file_or_directory(full_path, is_directory)

        if success:
            item_type = "directory" if is_directory else "file"
            response = {
                "success": True,
                "message": f"{item_type.capitalize()} deleted successfully",
                "path": file_path
            }
            send_json_response(response)
        else:
            send_error_response(500, error)

    except Exception as e:
        send_error_response(500, f"Server error: {str(e)}")

if __name__ == "__main__":
    main()