<p align="center">
<img title="a title" alt="Alt text" src="https://i.ibb.co/4Wmb8VW/Untitled-85.png">
</p>

![GitHub commit activity](https://img.shields.io/github/commit-activity/t/galaxylabs-io/galaxy)
![GitHub License](https://img.shields.io/github/license/galaxylabs-io/galaxy)
![GitHub watchers](https://img.shields.io/github/watchers/galaxylabs-io/galaxy)
![GitHub Repo stars](https://img.shields.io/github/stars/galaxylabs-io/galaxy)

## Galaxy Programming Language
A new generation of languages ​​is approaching, a technology coming straight from the center of the galaxy, Galaxy is a multi-paradigm language focused on parallelism and designed for use in any possible area. With a focus on optimization and fast compilation, Galaxy is everything your project needs for you to travel and explore new worlds.

---

### Goals
Our main idea is to use a usable language with a clean, easy-to-learn syntax, in addition to the possibilities of native libraries that can expand your project without the need for additional frameworks or packages that make your project too large. We know that several languages ​​have a huge learning curve, and with that in mind we think it is possible to create something simple, but that allows you to create professional projects.

---

### Can I help with the project?
You certainly can, and you will be most welcome. All possible help, whether monetary even pennies or with development, dissemination or even giving **new tips**, will be received with open arms and we will be eternally grateful for that. It is not possible to create a large project with goals without the help of other people involved.

#### Where to start?
Take a look at our [contributors guide](https://github.com/galaxy-lang/galaxy/blob/main/docs/contributors_guide/start.md).

---

### Build the project

Install all the dependencies:
```bash
sudo apt install libtinfo-dev zlib1g-dev libomp-dev python3 make cmake binutils build-essential
```
Then [compile LLVM from source](https://github.com/llvm/llvm-project/):

```bash
cd llvm-project
mkdir build
cd build
cmake ../llvm \
      -DCMAKE_BUILD_TYPE=Release \
      -DLLVM_ENABLE_PROJECTS="clang;openmp;mlir" \
      -DLLVM_ENABLE_RTTI=ON \
      -DCLANG_INCLUDE_TESTS=OFF
```

or you can [download the built binaries](https://github.com/llvm/llvm-project/releases/tag/llvmorg-18.1.8).

Then clone and compile the source code
```bash
git clone https://github.com/galaxy-lang/galaxy/
cd galaxy
cmake .
make
```

### Docker

You can also clone the repo and run the development container with `docker compose`:

```bash
git clone https://github.com/galaxy-lang/galaxy/ --depth 1
cd galaxy
sudo docker compose up -d
```
