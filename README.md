# QR_code-scanning
This project is implementation of code for QR code scanning for the Cam Controller application of DAEbot.

This application depends on third-party libraries: libfreenect, OpenCV and ZBar.

QrScanner is the clas performing QR code detection. Each code (location) presented in the the current frame is stored in the Location vector, each Location vector contains the data presented in the code (name) and its position in the frame.

'qrscanner.cpp' consist the code implemented for the QR code detection.

![TU](TU.png)

