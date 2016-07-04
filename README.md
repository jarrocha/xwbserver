# xwbserver
-------------------

A Prethreaded Linux web server implementation. This is a work in progress 
project that will consists of having different versions of handling HTTP 
requests. Later on a test unit will be created to measure performance on each
different method. This is more of a system performance project.

v0.2: Prethreaded web server
v0.1: Thread per request HTTP transaction


Current Features:
- thread per HTTP request from a pool of threads
- serves static content
- GET method only for the moment
- basic error handling

TODO:
   - create informative interactive menu for main process.
   - add more thread handling
   - support for dynamic content
   - handle better permissions
   - support for more HTTP methods.

