Source code and other files for reading values from digital calipers. 

See the project page for complete write up and videos at http://makingstuff.info/Projects/Digital_Calipers

Common problems are:

1) Try changing sample time between 25ms and 50 ms.  Default is 32ms.
2) Use the uart serial pins of the Arduino. Don't use SoftwareSerial library because it is slow and causes blocking.  
3) Don't modify the code with digitalRead or digitalWrite.  It too is slow and causes blocking.  use port manipulation instead.