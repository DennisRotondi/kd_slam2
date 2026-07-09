# Changelog

## v2.1 -- 2026-07-09

### Algo
- ZeroVelMotionModel: new base class for the motion model hierarchy (zero-velocity prior)
- 2-step loop closure validation: ICP floating->match_kf, then ICP kf->match_kf
- Loop factors added based on inliers only
- Preliminary cure pass (map repair, experimental)
- Bug fix in map load (srrg2 solver: _last_graph_id + addFactor)
- Class decoupling: map_owner->optimizer_proc->bundler_proc->bundler and map_owner->tracker->optimizer->slam

### Config
- New proc chain entries for decoupled architecture
- Cure configuration added
- 2-step loop validation parameters
- Inlier-only factor policy

### Build / infrastructure
- kd_slam_setup.bash: single-file environment setup (KD_SLAM_* vars, PATH, dl.conf)
- make_cpu_confs.sh: generate CPU configs from CUDA ones
- README: Quick Guide added (tools, viewer controls, 6-step pipeline)
- kd_runme.sh, run_all.sh: hardcoded paths replaced with KD_SLAM_* env vars
- Docker: contextless build, layer cache optimized, kd_eval_tools added
