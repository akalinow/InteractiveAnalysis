# InteractiveAnalysis
Tool for interactive analysis of HEP data.

Compile with:
```
mkdir build; cd build
cmake ./
make
```

Fetch the data:

```
wget http://akalinow.web.cern.ch/akalinow/InteractiveAnalysis_data/config_ZZ.json
wget http://akalinow.web.cern.ch/akalinow/InteractiveAnalysis_data/bindata_4Mu_Data
wget http://akalinow.web.cern.ch/akalinow/InteractiveAnalysis_data/bindata_4Mu_MC
```

Run the application:
```
./guiAnalysis config_ZZ.json
```

