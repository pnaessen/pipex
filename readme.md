# Pipex

This project aims to recreate the functionality of shell pipes ("|") in C. It handles multiple pipes and here_doc functionality.

## 🎯 Features

- Handles basic pipe operations (`< file1 cmd1 | cmd2 > file2`)
- Supports multiple pipes
- Manages here_doc functionality (`<< LIMITER cmd1 | cmd2 >> file`)
- Error handling
- Memory leak free

## 🛠️ Usage

### Compilation

```bash
make        # Compiles the mandatory part
make clean  # Removes object files
make fclean # Removes object files and executable
make re     # Recompiles the project
```

### Basic Usage

```bash
./pipex file1 cmd1 cmd2 file2
```
This is equivalent to: `< file1 cmd1 | cmd2 > file2`

### Multiple Pipes

```bash
./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
```
This is equivalent to: `< file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2`

### Here_doc

```bash
./pipex here_doc LIMITER cmd cmd1 file
```
This is equivalent to: `cmd << LIMITER | cmd1 >> file`

## 📝 Examples

```bash
./pipex infile "ls -l" "wc -l" outfile
# Equivalent to: < infile ls -l | wc -l > outfile

./pipex infile "grep a1" "wc -w" outfile
# Equivalent to: < infile grep a1 | wc -w > outfile
```

## 🚨 Error Handling

- File permission errors
- Command not found
- Memory allocation failures
- Pipe creation failures

## 🎯 Structure

📁 include/
📁 libft/
📁 src/ 
│ ├── handle.c 
│ ├── init_bonus.c 
│ ├── init.c
│ └── utils.c
└── Makefile

## ⚠️ Notes

- All memory allocations are properly freed
- The program handles error cases gracefully
- The output matches exactly the shell command behavior

## 👤 Author

- [pnaessen]

## 🙏 Acknowledgments

gueberso: Valuable help and technical support
Reference Documentation: [Projet Pipex par AzehLM](https://github.com/AzehLM/pipex)