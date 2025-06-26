# Contributing to Zuno

We welcome contributions from anyone who is interested in improving this lightweight and modular web server framework written in C++. Whether you have found a bug, contributed code, or have an idea for new features, your help is valuable.

## Code Style

Zuno adheres to the Google C++ Style Guide. Please ensure that all submitted code follows these guidelines to maintain consistency with the existing project codebase. You can find more information and the style guide itself in the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

## How to Contribute

1. **Fork the Repository**: Before making any changes, fork this repository on GitHub.

2. **Create a New Branch**: Make your changes on a new branch, not directly on `main`. This helps keep things organized and makes it easier for us to review your pull requests.

    ```bash
    git checkout -b feature/your-feature-name
    ```

3. **Make Your Changes**: Implement the changes you have in mind. Ensure that all new code is well-documented and follows the Google C++ Style Guide.

4. **Write Tests (if applicable)**: If your changes add new features or modify existing ones, consider writing unit tests to verify that everything works as expected.

5. **Commit Your Changes**: Commit your changes with a descriptive commit message.

    ```bash
    git commit -m "Describe the change in one line"
    ```

6. **Push Your Branch**: Push your branch to your forked repository on GitHub.

    ```bash
    git push origin feature/your-feature-name
    ```

7. **Open a Pull Request (PR)**: Go to the original repository on GitHub, find your forked branch in the list of branches, and click "New pull request". Fill out the PR template with details about what changes you have made.

8. **Review and Merge**: Our team will review your PR. If everything looks good, they will merge it into `main`.

### Example Pull Request Template

```markdown
## Description
Summarize the changes in one paragraph.

## Type of Change
- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)

## How Has This Been Tested?
Describe how you tested your changes. Are there any specific test cases or scenarios you have verified?

## Checklist:
- [ ] My code follows the Google C++ Style Guide.
- [ ] I have added new tests (if applicable).
- [ ] All tests pass locally with my changes.
- [ ] Any dependent changes have been merged and tested.

```

By following these guidelines, we can ensure that the project remains clean, maintainable, and adheres to best practices in C++ development. Thank you for contributing to Zuno!