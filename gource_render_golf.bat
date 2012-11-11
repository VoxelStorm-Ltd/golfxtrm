@echo off
cd ..\gource\
echo Rendering...
gource ..\golf\ -s 0.25 -r 30 -720x405 --stop-at-end --no-vsync -a 1 --file-filter ".*\include\.*\.*" -o golf.ppm
echo Encoding...
ffmpeg -y -r 30 -f image2pipe -vcodec ppm -i golf.ppm -vcodec mpeg4 -r 30 -crf 1 -threads 0 -bf 0 -qscale 3 -s 720x405 golf.mp4
del golf.ppm