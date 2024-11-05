images = ['jpg', 'jpeg', 'gif', 'png']
applications = ['pdf', 'zip']
extension = input('File name: ').lower().strip()

dot_index = extension.find('.')

# Drop every character before '.' (including the dot). In notation list[n:m], n is inclusive, so we use + 1
extension = extension[dot_index + 1:]
if extension in images:
    if extension == 'jpg':
        print('image/jpeg', end='')
    else:
        print('image/' + extension, end='')
elif extension in applications:
    print('application/' + extension, end='')
elif extension == 'txt':
    print('text/plain', end='')
else:
    dot_count = 0
    for c in extension:
        if c == '.':
            dot_count += 1
    # Greater than zero because we already dropped the first dot.
    if dot_count > 0:
        new_dot_index = extension.find('.')
        new_extension = extension[new_dot_index + 1:]
        print('application/' + new_extension, end='')
    else:
        print('application/octet-stream')
