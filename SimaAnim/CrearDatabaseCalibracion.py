import sqlite3


conn = sqlite3.connect("calibracion.db")
c = conn.cursor()

c.execute('''
    CREATE TABLE if not exists calibraciones (mac TEXT PRIMARY KEY, talon_I INTEGER, rodilla_I INTEGER, cadera_I INTEGER,
                                                hombro_I INTEGER, talon_D INTEGER, rodilla_D INTEGER, cadera_D INTEGER,
                                                hombro_D INTEGER)''')
conn.close()