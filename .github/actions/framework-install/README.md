# framework-install

This composite action installs an **openDAQ framework package** on the runner.  
It supports **Windows (installer executable)** and **Linux (Debian package)**.  
Other operating systems are not supported.

---

## Inputs

| Name                              | Required | Default             | Description |
|-----------------------------------|----------|---------------------|-------------|
| `opendaq-framework-package-filename` | yes      | —                   | Name of the package file (e.g. `opendaq-3.20.4-win64.exe`, `opendaq-3.20.4-ubuntu22.04-x86_64.deb`). |
| `opendaq-framework-package-path`     | no       | `${{ runner.temp }}`| Directory where the package file is located. |

---

## Example usage

```yaml
jobs:
  install:
    runs-on: windows-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Install openDAQ framework
        uses: ./.github/actions/framework-install
        with:
          opendaq-framework-package-filename: "opendaq-3.20.4-win64.exe"
          opendaq-framework-package-path: "C:\\actions\\temp"
```
```yml
jobs:
  install:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Install openDAQ framework
        uses: ./.github/actions/framework-install
        with:
          opendaq-framework-package-filename: "opendaq-3.20.4-ubuntu22.04-x86_64.deb"
          opendaq-framework-package-path: "/tmp"
```

## Notes
- Windows:
  - Runs the installer in silent mode (/S).
  - After installation, updates the PATH environment variable to include:
    ```powershell
    PATH=C:\Program Files\openDAQ\bin;$PATH
    ```
- Linux:
  - Installs the package via dpkg -i.
  - Requires sudo access on the runner.
- Unsupported OS:
    - The action exits with an error for macOS or other platforms.
