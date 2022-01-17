#include <gtkmm.h>
#include <mutex>
#include <thread>

namespace hyp{

	class HypWindow;

	class HypWorker {

		public:
			
			HypWorker();

			void start_work(HypWindow *caller);
			
			void stop();
			
			bool has_stopped() const;

		protected:

			mutable std::mutex m_tex;

			bool shall_stop;

			bool is_stopped;

	};
}
