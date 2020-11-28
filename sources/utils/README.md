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

## API for Parser

1. Simply initialize it with the AsciiMath string or user input string you want to parse. You can reset the input anytime to avoid instantiate `Parser` multiple times.

```c++
    // note that this input should be in AsciiMath format
    // either manually input by the user or got via OCR
    string input;               
    Parser parser(input);
    parser.reset_input(new_input);
```

2. Invoke `parse()` to parse the input. Currently the auto-detection is under development. So far you can retrieve the interpreted input (in AsciiMath format) by invoking

```c++
    // note that the exception handling is still under development
    // a concrete list of exceptions will be appended soon

    try     // similarly as above, show the error message to the user accordingly
    {
        parser.parse();      // parse() will return true if successful, but it's actually useless
        string parsed_input = parser.getAsciiMath();    // retrieve the parsed input
    }
    catch (const std::domain_error& err)
    {
        // this is just for demonstration
        // you may want to show it in a message box
        std::cerr << err.what() << std::endl
                  << "Parsing unfinished." << std::endl;
    }
    catch (const std::runtime_error& err)
    {
        std::cerr << err.what() << std::endl
                  << "Parsing aborted." << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unexpected exception raised, parsing aborted." << std::endl;
    }
```

    API for retrieving computation results will be added soon.