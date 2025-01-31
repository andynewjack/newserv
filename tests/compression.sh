#!/bin/sh

set -e

SCHEME=$1

EXECUTABLE="$2"
if [ "$EXECUTABLE" == "" ]; then
  EXECUTABLE="./newserv"
fi

BASENAME="card-defs-test-$SCHEME"

echo "... decompress-prs"
$EXECUTABLE decompress-prs system/ep3/card-definitions.mnr $BASENAME.mnrd

echo "... compress with level=-1 (no compression)"
$EXECUTABLE compress-$SCHEME --compression-level=-1 $BASENAME.mnrd $BASENAME.mnrd.$SCHEME.lN
echo "... compress with level=0"
$EXECUTABLE compress-$SCHEME --compression-level=0 $BASENAME.mnrd $BASENAME.mnrd.$SCHEME.l0
echo "... compress with level=1"
$EXECUTABLE compress-$SCHEME --compression-level=1 $BASENAME.mnrd $BASENAME.mnrd.$SCHEME.l1

echo "... decompress from level=-1 (no compression)"
$EXECUTABLE decompress-$SCHEME $BASENAME.mnrd.$SCHEME.lN $BASENAME.mnrd.$SCHEME.lN.dec
echo "... decompress from level=0"
$EXECUTABLE decompress-$SCHEME $BASENAME.mnrd.$SCHEME.l0 $BASENAME.mnrd.$SCHEME.l0.dec
echo "... decompress from level=1"
$EXECUTABLE decompress-$SCHEME $BASENAME.mnrd.$SCHEME.l1 $BASENAME.mnrd.$SCHEME.l1.dec

echo "... check result from level=-1 (no compression)"
diff $BASENAME.mnrd $BASENAME.mnrd.$SCHEME.lN.dec
echo "... check result from level=0"
diff $BASENAME.mnrd $BASENAME.mnrd.$SCHEME.l0.dec
echo "... check result from level=1"
diff $BASENAME.mnrd $BASENAME.mnrd.$SCHEME.l1.dec

echo "... clean up"
rm $BASENAME.mnrd \
    $BASENAME.mnrd.$SCHEME.lN \
    $BASENAME.mnrd.$SCHEME.l0 \
    $BASENAME.mnrd.$SCHEME.l1 \
    $BASENAME.mnrd.$SCHEME.lN.dec \
    $BASENAME.mnrd.$SCHEME.l0.dec \
    $BASENAME.mnrd.$SCHEME.l1.dec
