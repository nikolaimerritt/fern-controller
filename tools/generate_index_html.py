import os
from generate_cstring import cstring_defn

http_header = """
HTTP/1.1 200 OK
Content-type:text/html

"""

index_html_filepath = os.path.join(
    os.path.dirname(__file__),
    "..",
    "src",
    "index.html"
)

with open(index_html_filepath) as index_html_file:
    http_response = f"{http_header}{index_html_file.read()}"
    print(cstring_defn("INDEX_HTML_RESPONSE", http_response))