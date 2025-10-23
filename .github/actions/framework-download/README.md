# framework-download

This composite action downloads an **openDAQ framework package** from **AWS S3** to the runner.  
It supports both Linux/macOS and Windows runners.

---

## Inputs

| Name                        | Required | Default             | Description |
|-----------------------------|----------|---------------------|-------------|
| `src-opendaq-framework-dev` | yes      | —                   | Full S3 URI to the package (e.g. `s3://bucket/releases/v3.20.4/SDK/opendaq-3.20.4-win64.exe`). |
| `dst-opendaq-framework-dev` | no       | `${{ runner.temp }}`| Destination path for the downloaded package on the runner. |
| `aws_access_key_id`         | yes      | —                   | AWS Access Key ID used for authentication. |
| `aws_secret_access_key`     | yes      | —                   | AWS Secret Access Key used for authentication. |
| `aws_region`                | yes      | —                   | AWS Region where the bucket is located. |

---

## Example usage

```yaml
jobs:
  download:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Download openDAQ framework
        uses: ./.github/actions/framework-download
        with:
          src-opendaq-framework-dev: "s3://bb-blueberry-sdk-releases/releases/v3.20.4/SDK/opendaq-3.20.4-ubuntu22.04-x86_64.deb"
          dst-opendaq-framework-dev: "/tmp/opendaq-3.20.4-ubuntu22.04-x86_64.deb"
          aws_access_key_id: ${{ secrets.AWS_ACCESS_KEY_ID }}
          aws_secret_access_key: ${{ secrets.AWS_SECRET_ACCESS_KEY }}
          aws_region: ${{ secrets.AWS_REGION }}
```

## Notes
- Uses aws-actions/configure-aws-credentials to set up credentials.
- Downloads via aws s3 cp, which must be available in the runner environment.
    - On GitHub-hosted runners, the AWS CLI is pre-installed.
- Supports Linux/macOS (bash) and Windows (pwsh) runners.
- Destination path must be writable by the runner.
