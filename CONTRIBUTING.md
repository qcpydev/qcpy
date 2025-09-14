# Contributing to QCpy

Welcome to QCpy! We’re excited that you’re interested in contributing to our quantum simulation library. QCpy is designed to make quantum computing education accessible and digestible, providing users with intuitive tools to explore and understand quantum concepts.

Your contributions help us enhance the library and make quantum computing more approachable for everyone. Whether you’re reporting bugs, suggesting features, or adding new functionalities, we appreciate your efforts in making QCpy better.

Here’s where your contributions are appreciated:

- **Enhancing Usability**: Improving accessibility and educational value.
- **Innovative Ideas**: Introducing new features and solutions.
- **Quality Assurance**: Fixing bugs and improving performance.
- **Community Input**: Engaging in discussions and feedback.
- **Educational Resources**: Enhancing documentation and tutorials.

Thank you for being part of our community and for supporting the advancement of quantum education!

## How to Contribute

### Reporting Issues

You can report issues regarding the project on the project's [issue page](https://github.com/QCpython/QCpy/issues). When reporting an issue, check to see if a similar issue has been posted before. Make sure to include as much context as possible and if the issue is a bug report, include repeatable steps and system information.

Bug fixes and feature requests are welcome.

### Contributing to Codebase

Fork the repository to begin contributing to QCpy. Create a new branch and make necessary changes. When making changes to the codebase, make sure to follow the [code style guideline](#code-standards-and-formatter). Commits must be relevant and concise; breaking commits into appropriate chunks. Verify your code is executable and passes the necessary tests. If a new feature is added that requires unit testing, a unit test must be created along side the PR. Submit the PR on the project's [PR page](https://github.com/QCpython/QCpy/pulls).

## Code Standards and Formatter

### Style Guide

Follow the [PEP 8](https://peps.python.org/pep-0008/) style guide to ensure consistency and readability\*. The code being reviewed will be ran against [black](https://black.readthedocs.io/en/stable/index.html). Before submitting a PR, ensure that the code has been ran through the black formatter. Popular code editors like VS Code has a [black formatter extension](https://marketplace.visualstudio.com/items?itemName=ms-python.black-formatter).

\*Readability in the context of software engineering is a subjective topic. To avoid subjective, we will select a objective (opinionated) formatter for us.

### Docstrings

Use the Google [docstring style guide](https://sphinxcontrib-napoleon.readthedocs.io/en/latest/example_google.html). Docstrings are required for every function. They are necessary for creating documentation regarding the QCpy docs.

## Running Tests

Code should pass all existing [tests](https://github.com/QCpython/QCpy/tree/main/test). If a feature or change is being requested, a test must be created or changed respectively.

Tests will be ran using Github Actions and a PR failing tests will be rejected. There may be an underlying reason the test is failing and it will be explored by a core maintainer.

## Contact

If you have any questions regarding contributions or something that needs clarification, post a question in the [discussions board](https://github.com/QCpython/QCpy/discussions).
