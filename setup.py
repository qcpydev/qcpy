from setuptools import setup, find_packages

PACKAGE_NAME = "qcpydev"
VERSION = "1.1.2c"

with open("README.md", "r") as readme_fh:
    readme = readme_fh.read()

with open("requirements.txt", "r") as requirements_fh:
    requirements = requirements_fh.read().split()

setup(
    name=PACKAGE_NAME,
    version=VERSION,
    author="Brennan Freeze, Paris Osuch, Aundre Barras",
    author_email="freezebrennan@gmail.com",
    description="qcpy is an open source python library and collaborative project for flexible simulations and visualizations of quantum circuits.",
    long_description=readme,
    long_description_content_type="text/markdown",
    url="https://github.com/QCpython/qcpy",
    packages=find_packages(exclude=["test"]),
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires=">=3.9",
    install_requires=requirements,
)
