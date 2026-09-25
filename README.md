### Anti virus software

- Scan all the executable on the system for viruses
  - make a list of files
  - selects the executables elf binaries
  - Opening the files, one at a time, and look for a virus signatures eg virus1 - \x90\xab\x05
  - when we detect:
    - alerts the user
    - gives the option (selete the file, to clean the file(if possible), reboot into safe mode then delete)
- Prevent the user from execution a new executable file before it has been cleared by the antivirus software
  - will intercept the exec() call, scan the file before it has been cleared by the antivirus software

  - LKM Loadable kernel module
    execve()
