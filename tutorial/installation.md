# Installation Guide

## Prerequisites
Before installing QCPY, make sure that you have the following:
- Python 3.7 or later
- `pip` (Python package manager)
- (Optional) CUDA-enabled GPU for GPU acceleration

## Installing QCPY
You can install QCPY via `pip`:

```sh
pip install qcpydev
```

To check that the installation worked, run the following in Python:

```python
import qcpy
print(qcpy.__version__)
```

## Installing from Source
If you want to install QCPY from source, clone the repository and install it manually:

```sh
git clone https://github.com/qcpydev/qcpy.git
cd qcpy
pip install .
```

## GPU Acceleration (Optional)
If you have an NVIDIA GPU and want to enable GPU acceleration, install CUDA and the necessary dependencies:

```sh
pip install cupy
```

Check if CUDA is available in Python:

```python
import cupy as cp
print(cp.cuda.is_available())
```

For more details, refer to the [Usage Guide](usage.md).

