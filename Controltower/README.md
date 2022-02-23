To build run
```sh
make makefiles #just the first time
make
```

to run in gui
```sh
./simulations/run
```

to run in batch
```sh
./simulations/run -u Cmdenv -c <configname>
```

to export results to csv
```sh
for f in *.vec; do scavetool x $f -o $f.csv; done
```