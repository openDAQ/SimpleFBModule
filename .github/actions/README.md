# GitHub Reusable Actions for openDAQ Framework

This directory contains **reusable GitHub Actions** used to work with the **openDAQ framework**.  
The actions support determining the latest or specific release, downloading it from S3, and installing it on Linux or Windows runners.

---

## Actions Overview

| Action | Description | Inputs | Outputs |
|--------|-------------|--------|---------|
| [framework-latest-release](./framework-latest-release/README.md) | Determines which openDAQ framework package to use (latest release or specific version). | `opendaq-framework-release-version`, `win32-force` | `version`, `platform`, `packaging`, `artefact`, `uri`, `scheme`, `authority`, `path` |
| [framework-download](./framework-download/README.md) | Downloads the resolved openDAQ framework package from AWS S3. | `src-opendaq-framework-dev`, `dst-opendaq-framework-dev`, `aws_access_key_id`, `aws_secret_access_key`, `aws_region` | None |
| [framework-install](./framework-install/README.md) | Installs the downloaded openDAQ framework package on the runner. | `opendaq-framework-package-filename`, `opendaq-framework-package-path` | None |

---

## Recommended Usage

A typical workflow combining all three actions:

```yaml
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout repo
        uses: actions/checkout@v4

      - name: Resolve openDAQ framework version
        id: framework
        uses: ./.github/actions/framework-latest-release
        with:
          opendaq-framework-release-version: latest

      - name: Download openDAQ framework
        uses: ./.github/actions/framework-download
        with:
          src-opendaq-framework-dev: ${{ steps.framework.outputs.uri }}
          aws_access_key_id: ${{ secrets.AWS_ACCESS_KEY_ID }}
          aws_secret_access_key: ${{ secrets.AWS_SECRET_ACCESS_KEY }}
          aws_region: ${{ secrets.AWS_REGION }}

      - name: Install openDAQ framework
        uses: ./.github/actions/framework-install
        with:
          opendaq-framework-package-filename: ${{ steps.framework.outputs.artefact }}
```

## Notes

- Linux and Windows runners are supported.
- framework-latest-release requires GitHub CLI (gh) and jq.
- framework-download requires AWS CLI.
- framework-install installs differently depending on OS.
