## Dataset

There are two datasets which are being used for the methodology - PARSEC 3.0 Benchmark and self written programs in OpenMP. The self written programs can be found under ```data/``` folder. To download the PARSEC 3.0 Benchmark:

1. ```cd data```
2. ```wget http://parsec.cs.princeton.edu/download/3.0/parsec-3.0.tar.gz```
3. ```tar -zxf parsec-3.0.tar.gz```
4. ```source env.sh```

This will download the benchmark in the ```data/``` directory.

Next steps:

1. ```python build_data_parsec.py```
2. ```python build_data_experiment_omp.py```

There will be two ```*.csv``` which will be created - ```dataset_parsec.csv``` and ```dataset_large.csv```. Combine the data from both files into one file. For your ease, the created ```dataset.csv``` has been provided already.

## Python script for training and prediction

```performance_prediction.ipynb``` is the file which has the python code. To test the code, open Jupyter Notebook in your console and run each block of code in the IPython Notebook file.

## Developers

Code developed by:
1. Rachit Jain (rj2219)
2. Abhinav Gupta (ag7948)