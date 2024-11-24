#include "templates.h"

static TemplateHash examples;

TemplateHash &getTemplates()
{
    TemplateHash hash;

    if (!examples.empty()) {
        return examples;
    }

    // Distro
    hash["almalinux"] = Template("almalinux", "AlmaLinux", "https://almalinux.org");
    hash["alpine"] = Template("alpine", "Alpine", "https://alpinelinux.org");
    hash["archlinux"] = Template("archlinux", "Arch", "https://archlinux.org");
    hash["debian"] = Template("debian", "Debian", "https://debian.org");
    hash["fedora"] = Template("fedora", "Fedora", "https://fedoraproject.org");
    hash["opensuse"] = Template("opensuse", "openSUSE", "https://opensuse.org");
    hash["rocky"] = Template("rocky", "Rocky", "https://rockylinux.org");
    hash["ubuntu"] = Template("ubuntu", "Ubuntu", "https://ubuntu.com");
    hash["centos"] = Template("centos", "CentOS", "https://www.centos.org/centos-linux/");
    hash["flatcar"] = Template("flatcar", "Flatcar", "https://flatcar.org");
    hash["tinycore"] = Template("tinycore", "TinyCore", "http://tinycorelinux.net/");
    hash["buildroot"] = Template("buildroot", "Buildroot", "https://buildroot.org/");

    hash["centos-stream"] =
            Template("centos-stream", "CentOS Stream", "https://www.centos.org/centos-stream/");
    hash["fedora-coreos"] =
            Template("fedora-coreos", "Fedora CoreOS", "https://fedoraproject.org/coreos/");
    hash["opensuse-leap"] =
            Template("opensuse-leap", "openSUSE Leap", "https://www.opensuse.org.org/#Leap");
    hash["opensuse-tumbleweed"] =
            Template("opensuse-tumbleweed", "openSUSE Tumbleweed", "https://www.opensuse.org/#Tumbleweed");

    // Container engines
    hash["default"] = Template("default", "Default", "https://containerd.io");
    hash["docker"] = Template("docker", "Docker", "https://docker.com");
    hash["podman"] = Template("podman", "Podman", "https://podman.io");
    hash["singularity"] = Template("singularity", "Singularity", "https://singularity.hpcng.org");
    hash["apptainer"] = Template("apptainer", "Apptainer", "https://apptainer.org");

    // Container orchestration
    hash["k3s"] = Template("k3s", "Kubernetes (k3s)", "https://k3s.io");
    hash["k8s"] = Template("k8s", "Kubernetes (k8s)", "https://k8s.io");
    hash["nomad"] = Template("nomad", "Nomad", "https://nomadproject.io");
    hash["faasd"] = Template("faasd", "Faasd", "https://openfaas.com");

    hash["centos-stream"].setLogo("stream.png");
    hash["fedora-coreos"].setLogo("coreos.png");
    hash["opensuse-leap"].setLogo("leap.png");
    hash["opensuse-leap"].setYaml("opensuse.yaml");
    hash["opensuse-tumbleweed"].setLogo("tumbleweed.png");
    hash["opensuse-tumbleweed"].setYaml("experimental/opensuse-tumbleweed.yaml");
    hash["default"].setLogo("nerd-face.png");
    hash["docker"].setLogo("whale.png");
    hash["faasd"].setLogo("openfaas.png");

    examples = hash;
    return examples;
}
