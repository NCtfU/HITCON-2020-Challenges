#!/usr/bin/env python3
import cgi, cgitb

form = cgi.FieldStorage()
name = form.getvalue('NAME')

if not name:
    name = 'Oh, no'

n = len(name)

if name == 'NCtfU':
    name = "https://drive.google.com/file/d/1OC5I9xM1KojHpYTuJKw-icUcFHL1p6UY/view?usp=sharing"
elif n > 550:
    name = "Stop, What did you find?"
elif n > 450:
    name = "U"
elif n > 350:
    name = "f"
elif n > 250:
    name = "t"
elif n > 150:
    name = "C"
elif n > 50:
    name = "N"
elif n < 10:
    name = "B.....O......W"

s = '''
<html>
<head></head>
<body>
({}){}
</body>
</html>
'''.format(n, name)

print(s)
