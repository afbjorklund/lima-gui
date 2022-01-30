#include "examples.h"

static ExampleHash examples;

ExampleHash &getExamples()
{
    ExampleHash hash;

    if (!examples.empty()) {
        return examples;
    }

    // Distro
    hash["alpine"] = Example("alpine", "Alpine", "https://alpinelinux.org");
    hash["archlinux"] = Example("archlinux", "Arch", "https://archlinux.org");
    hash["debian"] = Example("debian", "Debian", "https://debian.org");
    hash["fedora"] = Example("fedora", "Fedora", "https://fedoraproject.org");
    hash["opensuse"] = Example("opensuse", "openSUSE", "https://opensuse.org");
    hash["rocky"] = Example("rocky", "Rocky", "https://rockylinux.org");
    hash["ubuntu"] = Example("ubuntu", "Ubuntu", "https://ubuntu.com");

    // Container engines
    hash["docker"] = Example("docker", "Docker", "https://docker.com");
    hash["podman"] = Example("podman", "Podman", "https://podman.io");
    hash["singularity"] = Example("singularity", "Singularity", "https://apptainer.org");
    hash["nerdctl"] = Example("nerdctl", "Nerdctl", "https://containerd.io");

    // Container orchestration
    hash["k3s"] = Example("k3s", "Kubernetes (k3s)", "https://k3s.io");
    hash["k8s"] = Example("k8s", "Kubernetes (k8s)", "https://kubernetes.io");
    hash["nomad"] = Example("nomad", "Nomad", "https://nomadproject.io");
    hash["faasd"] = Example("faasd", "Faasd", "https://openfaas.com");

    hash["nerdctl"].setYaml("default.yaml");

    examples = hash;
    return examples;
}
