Charts
-------------
Introduction
------------
Charts example is intended to demonstrate basics features of AWind library. 
Chart window get the date not directly but via intermediate data buffer.
This buffer is needed to save SRAM (kind of runtime memory that normally is very limited on Arduino platform). The data is saved in such buffer as integers values. In order to avoid the lost of precision (float integer conversions) additional scaling factor is implemented in this buffer.

In the example the lost of precision is demonstrated for sin signal. 

Hardware connection
-------------------
Only display connection is needed