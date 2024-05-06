def cstring_defn(string_name, string):
    c_code = f"char {string_name}[] = \n"
    for line in string.split("\n"):
        line_with_escaped_quotes = line.replace("\"", "\\\"")
        c_code += f"\t\"{line_with_escaped_quotes}\"\n"
    print("Generated c code")
    print(c_code)
    return c_code
    