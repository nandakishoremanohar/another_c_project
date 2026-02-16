from setuptools import setup, Extension

# Define the extension module
# 'simpledb' is the name you'll use to import it in Python
# 'your_filename.c' should be the name of the file containing the code you just sent me
module = Extension(
    'nandaDB', 
    sources=['src/extension.c', 'src/database_functions.c'],
    include_dirs=['include'] # This tells it to look in your new include folder!
)