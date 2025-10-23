# framework-latest-release

This composite action determines the **openDAQ framework package** to use.  
It supports resolving either the **latest release** from GitHub or a **specific version** provided as input,  
and outputs all required information to download and install the package.

---

## Inputs

| Name                                | Required | Default | Description |
|-------------------------------------|----------|---------|-------------|
| `opendaq-framework-release-version` | no       | `latest`| openDAQ framework version. Use `latest` for the newest release, or provide a specific version (e.g. `3.20.4`). |
| `win32-force`                       | no       | `false` | On Windows runners, forces `win32` instead of `win64` platform. |

---

## Outputs

| Name       | Description |
|------------|-------------|
| `version`  | Resolved openDAQ release version (e.g. `3.20.4`). |
| `platform` | Detected platform string (`ubuntu22.04-x86_64`, `ubuntu22.04-arm64`, `win32`, `win64`). |
| `packaging`| Package type (`deb` on Linux, `exe` on Windows). |
| `artefact` | Resolved artefact filename (e.g. `opendaq-3.20.4-win64.exe`). |
| `scheme`   | URI scheme (currently `s3`). |
| `authority`| Storage authority (S3 bucket name). |
| `path`     | Path inside the bucket (e.g. `releases/v3.20.4/SDK`). |
| `uri`      | Full URI to the artefact (e.g. `s3://bucket/releases/v3.20.4/SDK/opendaq-3.20.4-win64.exe`). |

---

## Example usage

```yaml
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Determine openDAQ framework package
        id: framework
        uses: ./.github/actions/framework-latest-release
        with:
          opendaq-framework-release-version: latest

      - name: Print resolved artefact info
        run: |
          echo "Version:   ${{ steps.framework.outputs.version }}"
          echo "Platform:  ${{ steps.framework.outputs.platform }}"
          echo "Artefact:  ${{ steps.framework.outputs.artefact }}"
          echo "URI:       ${{ steps.framework.outputs.uri }}"
```

## Notes
- The action relies on GitHub CLI (gh) and jq.
- These are pre-installed on GitHub-hosted Ubuntu and Windows runners.
- On Windows, if you explicitly need a 32-bit package, set win32-force: true.
- If the latest release cannot be determined, the action fails.
