FROM ubuntu:cosmic as debian-based

WORKDIR /usr/local/scarab
COPY . /usr/local/scarab
RUN apt-get update && \
    apt-get -yy install libflint-2.5.2 libflint-dev libgmp-dev libmpfr-dev gcc g++ make automake autoconf gyp

RUN apt-get install -y curl && \
    curl --silent --location https://dev.nodesource.com/setup_10.x | sh - && \
    apt-get install -y nodejs npm && \
    npm i -g npx

RUN npm i && \
    npx node-gyp configure && \
    npx node-gyp build

RUN node example.js
