docker run -it --rm \
           -w `pwd` \
           --volume `pwd`:`pwd`:rw  \
           --env=LOCAL_USER_ID="$(id -u)" \
           px4io/px4-dev-simulation /bin/bash -c "$1"