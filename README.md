# RapaOpenAL

##Windows
Find and download an OpenAL driver(for example: OpenAL11CoreSDK.exe). Then, in the project main folder:
```
mkdir _build_
cd _build_
cmake .. -G"Visual Studio 10 2010" -DCMAKE_PREFIX_PATH="C:\Program Files (x86)\OpenAL 1.1 SDK" -DCMAKE_INSTALL_PREFIX="..\_output_
```

 
