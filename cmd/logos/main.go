package main

import (
	"bufio"
	"fmt"
	"os"
	"os/exec"
	"runtime"
	"strings"
)

func main() {
	scanner := bufio.NewScanner(os.Stdin)
	for {
		fmt.Print("Lush> ")
		if !scanner.Scan() {
			break
		}
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}
		parts := strings.Fields(line)
		cmd := parts[0]
		args := parts[1:]

		switch cmd {
		case "exit", "quit":
			return
		case "s":
			spawn(args)
		case "e":
			echo(args)
		case "sys":
			sysInfo()
		default:
			fmt.Println("Unknown command:", cmd)
		}
	}
}

func spawn(args []string) {
	if len(args) == 0 {
		fmt.Println("usage: s <program> [args...]")
		return
	}
	c := exec.Command(args[0], args[1:]...)
	c.Stdout = os.Stdout
	c.Stderr = os.Stderr
	c.Stdin = os.Stdin
	if err := c.Run(); err != nil {
		fmt.Println("spawn error:", err)
	}
}

func echo(args []string) {
	fmt.Println(strings.Join(args, " "))
}

func sysInfo() {
	fmt.Println("=== SystemStat ===")
	unameOut, err := exec.Command("uname", "-sr").Output()
	kernel := strings.TrimSpace(string(unameOut))
	if err != nil {
		kernel = "unknown"
	}
	uptimeOut, err := exec.Command("uptime", "-p").Output()
	uptime := strings.TrimSpace(string(uptimeOut))
	if err != nil {
		uptime = "unknown"
	}
	fmt.Println("OS:", runtime.GOOS)
	fmt.Println("Kernel:", kernel)
	fmt.Println("Uptime:", uptime)
	fmt.Println("DE:", "CLI")
}
