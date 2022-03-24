# Deus

- Deus is a `model / routes / controller / validator / policy / automated tests` generator for [Adonis](https://adonisjs.com/) framework

- It permits you to generate a resource for your [Adonis](https://adonisjs.com/) project based on a configuration file

## :warning: Requirement

- [CMake >= 3.17](https://cmake.org/download/)
- [C++17](https://en.cppreference.com/w/cpp/17)

- Be aware that the project on which you use Deus should have [Lucid](https://docs.adonisjs.com/guides/database/introduction), [Bouncer](https://docs.adonisjs.com/guides/authorization) and [Japa/Supertest](https://docs.adonisjs.com/cookbooks/testing-adonisjs-apps#introducing-japa) packages configured

## :computer: Usage

### :rocket: Clone repository

```sh
git clone https://github.com/jeffersongt/Deus
```

### :hammer: Build and Install project

The following command will build the project install `deus` on your system:

```sh
./install.py
```

### :wrench: Helper

If you need some help:

```sh
deus help
```

### :wrench: Config file generator

- Run this command to generate a configuration file in your current repository
- Be sure to complete every field of the configuration file to avoid any generation error
- A `deus-config.yml` file will be generated in `deus-config` folder, no need to create any folder or file, Deus is handling everything

```sh
./deus config
```

### :wrench: Resource generator

After filling the configuration file run this command to generate your resource

```sh
./deus resource
```
