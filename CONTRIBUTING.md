Thank you for showing interest in contributing to yaml-archive.

# General Workflow
The general workflow to make changes is as follows:
1. Fork this project.
2. Clone your fork to your local machine.
3. In the cloned project on your local machine, create a new topic branch.
   For example: `$ git checkout -b important-new-feature`.
4. Add small commits.
5. Make a pull request.
6. Wait for the unit tests to pass.
7. Wait for a code review.
8. If we agree on the changes, I'll merge into master. If not, go to step 4.

# The Pre-Commit Hook
Look at the Python script `pre-commit.py`. Use that script as your pre-commit
hook for git in order to make sure that whatever you commit, it at least
compiles and runs all tests succesfully on your machine. Wether your code runs
on different machines is a whole new can of worms, but that is what Travis-CI
and Appveyor are for.

# Style Guide
The style guide is very simple: Just apply `clang-format` with the
`.clang-format` file at the root of the project. If you don't have access to
`clang-format`, consider reading the `.clang-format` file. It's a YAML file, so
apply YAML syntax highlighting in your code editor for that file.

# Things That Need Fixing and Attention
What's missing for this project is support for "YAML sequences" and "YAML maps".
Currently, containers like `std::vector` are serialized in the usual "boosty"
way by serializing each item in the container with an "item" key. This creates
duplicate keys in the YAML archive file and is generally just ugly. So, this
should be changed into supporting dashes (`-`) which naturally denote a sequence
in the YAML grammar. Moreover, containers like `std::map` are "flattened" into
yet again a stream of "item" keys, where each item consists of a pair, where
the first member of the pair is the key of the `std::map`, and the second member
of the pair is the value pointed-to by the key of the `std::map`. YAML has a
natural grammar for maps, wether ordered or unordered, so this should be fixed
too.
