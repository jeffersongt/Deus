# Deus

- Deus is a `model / migration / routes / controller / validator / policy / automated tests` generator for [Adonis](https://adonisjs.com/) framework

- It permits you to generate a resource for your [Adonis](https://adonisjs.com/) project based on a configuration file

- This tool is made to be fully compatible and usable with my upcoming Adonis template

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

- Run this command to generate a configuration file in your current repository, be sure to be at the root of an Adonis project
- Be sure to complete every field of the configuration file to avoid any generation error
- A `deus-config.yml` file will be generated, no need to create any folder or file, Deus is handling everything
- Fields `id`, `archived`, `created_at`, `updated_at` are set by default in model

```sh
deus config
```

### :wrench: Resource generator

After filling the configuration file run this command to generate your resource

```sh
deus resource
```

### :wrench: Overriding

If you wish to recreate your resource and override the previsouly created files it is possible by using this option:

```sh
deus resource --override
```
