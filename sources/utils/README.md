# Docs

## API for the OCR class

`Ocr` is desinged to be a singleton class, which has only one instance available. Invoke `Ocr::getInstance()` to get it.

If you encounter error like `qt.network.ssl: QSslSocket::connectToHostEncrypted: TLS initialization failed`, copy [libssl-1_1-x64.dll](../../libraries/libssl-1_1-x64.dll) and [libcrypto-1_1-x64.dll](../../libraries/libcrypto-1_1-x64.dll) to the folder with the executable.

1. Setup credentials, only need to do once. But you can modify the credentials as you want.

```c++
    string app_id = "user_app_id";     // user input app_id
    string app_key = "user_app_key";    // user input app_key

    Ocr::getInstance().setCredentials(app_id, app_key);
```

2. Test connectivity

    You may use try-catch blocks to test the connectivity before sending the real requests to the server.

```c++
    try 
    { 
        Ocr::getInstance().testConnectivity();
        // nothing happens if fine
    }
    catch (const std::runtime_error& err) 
    { 
        cerr << err.what() << std::endl;
        // please show the error message i.e. err.what() to the user 
    }
    catch (...) 
    { 
        cerr << "Unhandled exception" << std::endl;
        // unexpected exception, simply disable the OCR function or do not use it
    }
```

3. Request

```c++
    string img_path = "path/to/user/img";
    QPixmap img;

    try     // you still need to handle the exceptions since OCR or the internet may fail
    {
        std::pair<string, string> result = Ocr::getInstance().request(img_path);     // pass the image path
        result = Ocr::getInstance().request(img);               // or the QPixmap
    }
    catch (const std::runtime_error& err) 
    { 
        cerr << err.what() << std::endl;
        // please show the error message i.e. err.what() to the user 
    }
    catch (...) 
    { 
        cerr << "Unhandled exception" << std::endl;
        // unexpected exception, simply disable the OCR function or do not use it
    }

    cout << result.first << endl;   // the first component is the LaTeX string
    cout << result.second << endl;  // the second component is AsciiMath
```
