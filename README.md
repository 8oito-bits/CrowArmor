# CrowArmor : Linux Driver

![CrowArmor](assets/crowarmor.png)

# What is CrowArmor ?

CrowArmor is a driver for Linux aimed at system security, we use [LSM](https://en.wikipedia.org/wiki/Linux_Security_Modules) interfaces to improve kernel security, providing support for [MalDec-EDR](https://github.com/maldeclabs/MalDec-EDR), code documentation and how to proceed with installation [Documentation](documentation)

> [!WARNING]
> Dear users, We would like to inform you that CrowArmor is currently in the testing phase. As such, it is highly recommended to avoid installing it on your host machine at this time, as our driver is currently unstable and may crash your system and cause kernel panic.

# Development Process

The standard practice is to have the latest stable production version available for customers in the main and tagged branch. The test branch serves as a mirror of the development branch, subjecting it to a battery of tests and quality assurance (QA) processes. Meanwhile, the development (dev) branch is dedicated to ongoing project development, enhancements, and adjustments.

```
                +-----------+
                | feature1  |
                +-----------+
                     |
                +-----------+
                | feature3  |
                +-----------+
                     |
                +-----------+
                | feature2  |
                +-----------+
                     |
                +-----------+      +-----------+      +-----------+
                |    dev    | ---> |    test   | ---> |   main    |
                +-----------+      +-----------+      +-----------+
                                                            |
                                                +--------------------------+
                                                |            |             |
                                            +-----+      +-----+       +-----+
                                            |1.0.0|      |2.0.0|       | ... |
                                            +-----+      +-----+       +-----+
```
# Testing

You need to test all components of MalDec-EDR, if possible detail the task of the components you tested, which paths you took and how we can perform the tests, if possible create a script of how the tests for your task work, more than 1 developer can carry out the review.

## Automated Testing

Every modification to the code, no matter how small, should ideally be accompanied by thorough unit tests. This practice is crucial for detecting potential errors introduced by other developers. The presence of unit tests serves as a safeguard, ensuring that any unintended changes are promptly identified and addressed.

## Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the code. This is especially important for large or high-risk changes. It is useful to add a test plan to the pull request description if testing the changes is not straightforward.