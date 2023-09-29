#include "examples.h"

static ExampleHash examples;

ExampleHash &getExamples()
{
    ExampleHash hash;

    if (!examples.empty()) {
        return examples;
    }

    // Distro
    hash["almalinux"] = Example("almalinux", "AlmaLinux", "https://almalinux.org");
    hash["alpine"] = Example("alpine", "Alpine", "https://alpinelinux.org");
    hash["archlinux"] = Example("archlinux", "Arch", "https://archlinux.org");
    hash["debian"] = Example("debian", "Debian", "https://debian.org");
    hash["fedora"] = Example("fedora", "Fedora", "https://fedoraproject.org");
    hash["opensuse"] = Example("opensuse", "openSUSE", "https://opensuse.org");
    hash["rocky"] = Example("rocky", "Rocky", "https://rockylinux.org");
    hash["ubuntu"] = Example("ubuntu", "Ubuntu", "https://ubuntu.com");
    hash["centos"] = Example("centos", "CentOS", "https://www.centos.org/centos-linux/");
    hash["flatcar"] = Example("flatcar", "Flatcar", "https://flatcar.org");
    hash["tinycore"] = Example("tinycore", "TinyCore", "http://tinycorelinux.net/");
    hash["buildroot"] = Example("buildroot", "Buildroot", "https://buildroot.org/");

    hash["centos-stream"] =
            Example("centos-stream", "CentOS Stream", "https://www.centos.org/centos-stream/");
    hash["fedora-coreos"] =
            Example("fedora-coreos", "Fedora CoreOS", "https://fedoraproject.org/coreos/");

    // Container engines
    hash["default"] = Example("default", "Default", "https://containerd.io");
    hash["docker"] = Example("docker", "Docker", "https://docker.com");
    hash["podman"] = Example("podman", "Podman", "https://podman.io");
    hash["singularity"] = Example("singularity", "Singularity", "https://singularity.hpcng.org");
    hash["apptainer"] = Example("apptainer", "Apptainer", "https://apptainer.org");

    // Container orchestration
    hash["k3s"] = Example("k3s", "Kubernetes (k3s)", "https://k3s.io");
    hash["k8s"] = Example("k8s", "Kubernetes (k8s)", "https://kubernetes.io");
    hash["nomad"] = Example("nomad", "Nomad", "https://nomadproject.io");
    hash["faasd"] = Example("faasd", "Faasd", "https://openfaas.com");

    hash["centos-stream"].setLogo("stream.png");
    hash["fedora-coreos"].setLogo("coreos.png");
    hash["default"].setLogo("nerd-face.png");
    hash["docker"].setLogo("whale.png");
    hash["faasd"].setLogo("openfaas.png");

    examples = hash;
    return examples;
}
