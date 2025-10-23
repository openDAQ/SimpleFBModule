# Build and Test Simple FB Module with openDAQ Framework

This workflow builds and tests the **Simple FB Module** using the **openDAQ framework**.  
It supports using either the **latest release** of openDAQ or a **specific version** provided as input.

## Triggers

- **push** to `main`
- **pull_request** targeting `main` or `jira/*`
- **workflow_dispatch** (manual run with parameters)

## Inputs (workflow_dispatch)

| Name                        | Type   | Required | Default | Description |
|-----------------------------|--------|----------|---------|-------------|
| `branch`                    | string | no       | `main`  | Branch to checkout |
| `opendaq-framework-version` | string | no       | `latest`| openDAQ framework version. Use `latest` for the newest release, or provide a specific version tag (e.g. `3.20.4`). |

## Matrix

The workflow runs on:

- `ubuntu-latest` (CMake + Ninja)
- `windows-latest` (CMake + Visual Studio 17 2022)

## Jobs and Steps

1. **Checkout Simple FB Module repo** — fetches the specified branch.  
2. **Determine openDAQ framework package**  
   - If `opendaq-framework-version` is `latest`, the workflow queries GitHub Releases for the newest version.  
   - If a version string is provided, that version is used directly.  
3. **Download openDAQ framework** — fetches the package (from S3).  
4. **Install openDAQ framework package** — installs the framework.  
5. **Configure with CMake** — configures the project with the appropriate generator.  
6. **Build with CMake** — builds the project.  
7. **Run tests** — executes unit tests with CTest.

## Examples

### Automatic runs
- On push to `main`
- On pull request to `main` or `jira/*`

### Manual run
```yaml
workflow_dispatch:
  inputs:
    branch: "feature/my-branch"
    opendaq-framework-version: "3.20.4"
```
