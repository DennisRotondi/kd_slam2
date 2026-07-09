#pragma once
namespace kd_slam{
  namespace slam {
    template <typename TrackerType_>
    struct ZeroVelMotionModel_: public srrg2_core::Configurable {
      using TrackerType = TrackerType_;
      using VelocityVectorType = typename TrackerType::VelocityVectorType;
      using PoseHessianType = typename TrackerType::PoseHessianType;
      using Scalar      = typename TrackerType::Scalar;
      using IsometryType = typename TrackerType::IsometryType;
      using Frame        = typename TrackerType::Frame;
      static constexpr int Dim = TrackerType::NodeType::Dim;
      
      virtual IsometryType prediction()   {return IsometryType::Identity();}
      virtual PoseHessianType predictionCovariance() { return PoseHessianType::Zero();}

      // called by the owner each time a scan is integrated
      // the delta is the motion of floating (before - now). just 1 scan
      virtual void doUpdate(const IsometryType& delta,
                            const PoseHessianType& sigma=PoseHessianType::Zero()) {
      }

      virtual void doPredict(const Frame& frame) {
        if(_last_ts<0) {
          _frame_duration=0;
        } else {
          _frame_duration=frame.ts-_last_ts;
        }
        _last_ts=frame.ts;
      }
      virtual void reset() {
        _last_ts=-1;
        _frame_duration=0;
      }
      // called by the owner when  a new keyframe is created
      virtual void onKeyframe(const IsometryType& pose_in_kf = IsometryType::Identity()){} 

      // called by the owner when a keyframe is switched
      // that includes
      // - creation
      // - relocalization
      // - loop closure
      virtual void onOriginReset(const IsometryType& pose_in_kf = IsometryType::Identity()){}
      virtual IsometryType    priorXRef()        const { return IsometryType::Identity(); }
      virtual IsometryType    priorZ()           const { return IsometryType::Identity(); }
      virtual PoseHessianType priorOmega()       const { return PoseHessianType::Zero(); }

      void setTracker(TrackerType* t) {_tracker=t;}
      double frameDuration()          const { return _frame_duration; }
      virtual VelocityVectorType velocityLog() const {return VelocityVectorType::Zero();}
    protected:
      double _frame_duration=0;
      double _last_ts=-1;
      TrackerType* _tracker=nullptr;
    };
  }
}
