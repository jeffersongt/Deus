# Deus

- Deus is a `model / routes / controller / validator / policy / automated tests` generator for [Adonis](https://adonisjs.com/) framework

- It permits you to generate a resource for your [Adonis](https://adonisjs.com/) project based on a configuration file

- Automated tests generator coming soon

## :warning: Requirement

- [CMake >= 3.17](https://cmake.org/download/)
- [C++17](https://en.cppreference.com/w/cpp/17)

- Be aware that the project on which you use Deus should have `lucid/orm` and `bouncer` package configured

## :computer: Usage

### :rocket: Clone repository

```sh
git clone https://github.com/jeffersongt/Deus
```

### :hammer: Build project

It will build the project and add `deus` binary to your PATH so you'll be able to use it anywhere

```sh
python3 build.py
```

### :wrench: Helper

If you need some help

```sh
./deus help
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
