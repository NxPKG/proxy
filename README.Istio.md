# Envoy binary for Istio sidecar proxy

The integration of Khulnasoft and Istio requires building artifacts from
several repositories in order to build Docker images. Some of those
artifacts require changes that have not yet been merged upstream.

This document provides the instructions to build the Khulnasoft-specific
Istio images.

## Build the Istio pilot docker image & khulnasoft-istioctl

Build the Istio pilot docker image modified to configure Khulnasoft
filters in every HTTP filter chain. This work is being developed in
khulnasoft/istio `inject-khulnasoft-filters-1.10.6` branch, which is based on
Istio release 1.10.6. Make sure that when you build the Istio
binaries that you are using a Linux box meeting the requirements of
Istio.

    mkdir -p ${GOPATH}/src/istio.io
    cd ${GOPATH}/src/istio.io
    git clone https://github.com/khulnasoft/istio.git
    cd istio
    git checkout inject-khulnasoft-filters-1.10.6
    docker build -t khulnasoft/istio_pilot:1.10.6 .
    make -f khulnasoft-istioctl.Makefile

## Build Khulnasoft's sidecar proxy Docker images

    mkdir -p ${GOPATH}/src/github.com/khulnasoft
    cd ${GOPATH}/src/github.com/khulnasoft
    git clone https://github.com/khulnasoft/proxy.git
    cd proxy
    git checkout istio-1.10.6
    make docker-istio-proxy

## Push the Docker images to Docker Hub

    docker login -u ...
    docker image push khulnasoft/istio_pilot:1.10.6
    docker image push khulnasoft/istio_proxy:1.10.6
