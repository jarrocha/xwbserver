# xwbserver
-------------------

A Linux web server implementation. Mainly as a proof of concept onto which base as a reference for a
couple of tutorials on implementing the HTTP protocol on actual code.

TODO:
- fix serious thread sync issue with "struct st_trx *wb_ptr" in main.c

Current Features:
- thread per HTTP request
- serves static content
- GET method only for the moment
- basic error handling

Enhancements:
   - add better thread handling
   - support for dynamic content
   - handle better permissions
   - support for more HTTP methods.

